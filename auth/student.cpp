#include "auth/student.h"

#include "db/database.h"
#include "learn/course_registration.h"
#include "utils/exceptions.h"
#include "utils/split_string.h"
#include "utils/vector.h"
#include <iostream>
#include <learn/course.h>

namespace auth {
student::student (std::string name, std::string faculty, std::string email, std::string password)
: user (name, faculty, email, password, Role::STUDENT) {
}
student::student (std::string id) : user{ id } {
    get ();
}
bool student::add_to_database (SQLite::Database& db) {
    if (saved_in_db ())
        return true;

    SQLite::Statement query (
    db, "INSERT INTO Users(name, password_hash, email, faculty, role) VALUES(?,?, ?, ?, ?) RETURNING id");

    query.bindNoCopy (1, _name);
    query.bindNoCopy (2, _password_hash);
    query.bindNoCopy (3, _email);
    query.bindNoCopy (4, _faculty);
    query.bindNoCopy (5, role_to_string (_role));

    query.executeStep ();
    int user_id = query.getColumn (0);


    query = SQLite::Statement (db, "SELECT id FROM Students ORDER BY id DESC LIMIT 1");
    query.executeStep ();
    std::string last_student_id = query.getColumn (0);

    std::string sub1   = last_student_id.substr (0, 5);
    std::string sub2   = last_student_id.substr (5);
    std::string new_id = sub1 + std::to_string (std::stoi (sub2) + 1);

    query = SQLite::Statement (db, "INSERT INTO Students(id, user_id) VALUES(?,?)");
    query.bind (1, new_id);
    query.bind (2, user_id);

    int success = query.exec ();
    _id         = new_id;
    _user_id    = std::to_string (user_id);
    return success;
}
void student::register_course (std::string course) {
    // validate (if course is not already registered)
    for (int i = 0; i < _courses_registrations.size (); i++) {
        if (course == _courses_registrations[i]) {
            // todo
            // Get from database and check if dropped
        }
    }
    // adding to database and member variable
    learn::course_registration registration (course, _id);
    _courses_registrations.push_back (course);
    db::database::get_instance ().add_item (registration);
}


void student::drop_course (const std::string& course) {
    // checks if course is even registered.
    for (int i = 0; i < _courses_registrations.size (); i++) {
        if (course == _courses_registrations[i]) {
            _courses_registrations.erase (i);
            // Todo
            // db::database::get_instance().remove_item(*this);
            return;
        }
    }
    throw utils::custom_exception{ "Course not registered" };
}

bool student::remove_from_database (SQLite::Database& db) {
    SQLite::Statement query (db,
    "DELETE FROM Users WHERE Users.id = (SELECT Users.id FROM Users JOIN "
    "Students ON Students.user_id == Users.id WHERE Students.id = ?)");
    query.bindNoCopy (1, _id);

    int success = query.exec ();
    return success;
}

bool student::update_in_database (SQLite::Database& db,
std::map<std::string, std::any> props) {
    std::string query_string = "UPDATE Users SET ";
    int count                = 0;
    for (auto const& [key, val] : props) {
        if (count > 0)
            query_string += ",";
        query_string += key + " = '" + std::any_cast<std::string> (val) + "'";
        count++;
    }

    query_string +=
    " WHERE id = (SELECT Users.id FROM Users JOIN "
    "Students ON Students.user_id == Users.id WHERE Students.id = ?)";

    SQLite::Statement query (db, query_string);
    query.bindNoCopy (1, _id);

    int success = query.exec ();
    return success;
}

utils::vector<std::unique_ptr<student>> student::getStudents (
std::map<std::string, std::string> props) {
    std::string query_string = "select students.id from students join users on "
                               "users.id == students.user_id";

    utils::vector<std::unique_ptr<student>> results;

    SQLite::Statement query (db::database::get_db (), query_string);

    if (!props.empty ()) {
        int ind = props.size () - 1;
        query_string += " where ";
        for (auto const& p : props) {
            query_string += p.first + " = ?" + (ind > 0 ? " and " : "");
            ind--;
        }

        query = SQLite::Statement (db::database::get_db (), query_string);
        int i = 1;

        for (auto const& p : props) {
            query.bind (i++, p.second);
        }
    }


    while (query.executeStep ()) {
        std::string student_id = (std::string)query.getColumn (0);
        std::unique_ptr<student> s (new student (student_id));
        results.push_back (std::move (s));
    }

    return results;
}

void student::get () {
    if (!saved_in_db ())
        throw utils::custom_exception{ "Item not saved in database;" };

    std::string query_string =
    "select users.*, (select group_concat(students_courses.id) from "
    "students_courses where students_Courses.student_id = students.id) as "
    "courses_registrations from students join users on users.id == "
    "students.user_id where students.id = ?";

    SQLite::Statement query (db::database::get_db (), query_string);
    query.bind (1, _id);

    if (!query.executeStep ()) {
        std::cout << _id << std::endl;
        throw utils::custom_exception ("Invalid ID for student");
    }
    _user_id                                = (std::string)query.getColumn (0);
    _password_hash                          = (std::string)query.getColumn (1);
    _email                                  = (std::string)query.getColumn (2);
    _faculty                                = (std::string)query.getColumn (3);
    _name                                   = (std::string)query.getColumn (4);
    std::string role_string                 = (std::string)query.getColumn (5);
    _role                                   = string_to_role (role_string);
    std::string course_registrations_string = (std::string)query.getColumn (6);
    _courses_registrations = utils::split_string (course_registrations_string, ',');

#if PRINT_DATA_WHEN_RETRIEVED
    std::cout << "Student: " << _name << "\nID: " << _id << "\nEmail: " << _email
              << "\nFaculty: " << _faculty << "\nRole: " << role_string
              << "\nCourses: " << course_registrations_string << std::endl
              << std::endl;
#endif
}

utils::vector<std::unique_ptr<learn::course>> student::get_courses () {
    utils::vector<std::unique_ptr<learn::course>> courses;
    for (auto& course_reg : _courses_registrations) {
        auto reg = learn::course_registration (course_reg).get_course ();
        std::unique_ptr<learn::course> course =
        std::make_unique<learn::course> (learn::course (reg.get_name ()));
        courses.push_back (std::move (course));
    }
    return courses;
}
} // namespace auth
