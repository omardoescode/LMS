#include "auth/instructor.h"

#include "utils/split_string.h"

#include <iostream>

namespace auth {

instructor::instructor (std::string id) : user (id) {
    get ();
}

instructor::instructor (std::string name,
std::string faculty,
std::string email,
std::string password,
bool is_teaching_assistant)
: user (name, faculty, email, password, Role::INSTRUCTOR),
  _is_teaching_assistant (is_teaching_assistant) {
}
void instructor::get () {
    if (!saved_in_db ())
        throw utils::custom_exception{ "Item not saved in database;" };

    std::string query_string =
    "select users.*, instructors.id as instructor_id, "
    "instructors.is_teaching_assistant, (select "
    "group_concat(instructors_courses.course_id) from instructors_courses "
    "where instructors_courses.instructor_id = instructors.id) as courses from "
    "instructors join users on users.id == instructors.user_id where "
    "instructors.id = ?";

    SQLite::Statement query (db::database::get_db (), query_string);
    query.bind (1, _id);

    while (query.executeStep ()) {

        _password_hash          = (std::string)query.getColumn (1);
        _email                  = (std::string)query.getColumn (2);
        _faculty                = (std::string)query.getColumn (3);
        _name                   = (std::string)query.getColumn (4);
        std::string role_string = (std::string)query.getColumn (5);
        _role                   = string_to_role (role_string);
        _is_teaching_assistant  = query.getColumn (7).getInt ();
        std::string courses_ids = query.getColumn (8);
        _courses                = utils::split_string (courses_ids, ',');

#if PRINT_DATA_WHEN_RETRIEVED
        std::cout
        << "Instructor: " << _name << "\nID: " << _id << "\nEmail: " << _email
        << "\nFaculty: " << _faculty
        << "\nTeaching Assistant: " << (_is_teaching_assistant ? "Yes" : "No")
        << "\nRole: " << role_string << "\nCourses: " << courses_ids << std::endl
        << std::endl;
#endif
    }
}
bool instructor::add_to_database (SQLite::Database& db) {
    SQLite::Statement query (
    db, "INSERT INTO Users(name, password_hash, email, faculty, role) VALUES(?,?, ?, ?, ?) RETURNING id");

    query.bindNoCopy (1, _name);
    query.bindNoCopy (2, _password_hash);
    query.bindNoCopy (3, _email);
    query.bindNoCopy (4, _faculty);
    query.bindNoCopy (5, role_to_string (_role));

    query.executeStep ();
    int user_id = query.getColumn (0);

    std::string id = _name;
    std::replace (id.begin (), id.end (), ' ', '.');

    query = SQLite::Statement (
    db, "INSERT INTO Instructors(id, user_id, is_teaching_assistant) VALUES(?,?,?)");
    query.bind (1, id);
    query.bind (2, user_id);
    query.bind (3, _is_teaching_assistant);

    int success = query.exec ();
    _id         = id;
    return success;
}

bool instructor::add_course (std::string course_id) {
    SQLite::Statement query (db::database::get_instance ().get_db (),
    "INSERT INTO Instructors_Courses(instructor_id, course_id) VALUES(?,?) "
    "RETURNING id");

    query.bindNoCopy (1, _id);
    query.bindNoCopy (2, course_id);

    query.executeStep ();
    int course_reg_id = query.getColumn (0);

    _id = course_reg_id;
    return (course_reg_id != 0);
}

bool instructor::remove_course (std::string course_id) {
    SQLite::Statement query (db::database::get_instance ().get_db (),
    "DELETE FROM Instructors_Courses WHERE instructor_id = ?, course_id = ?");

    query.bindNoCopy (1, _id);
    query.bindNoCopy (2, course_id);

    int success = query.exec ();
    return success;
}


bool instructor::remove_from_database (SQLite::Database& db) {
    SQLite::Statement query (db,
    "DELETE FROM Users WHERE Users.id = (SELECT Users.id FROM Users JOIN "
    "Instructors ON Instructors.user_id == Users.id WHERE Instructors.id = ?)");
    query.bindNoCopy (1, _id);

    int success = query.exec ();
    return success;
}
bool instructor::update_in_database (SQLite::Database& db,
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
    "Insturctors ON Insturctors.user_id == Users.id WHERE Insturctors.id = ?)";

    SQLite::Statement query (db, query_string);
    query.bindNoCopy (1, _id);

    int success = query.exec ();
    return success;
}
} // namespace auth
