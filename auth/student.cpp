#include "auth/student.h"

#include "db/database.h"
#include "learn/course_registration.h"
#include "utils/exceptions.h"
#include "utils/vector.h"

#include <iostream>

namespace auth {
student::student (std::string username, std::string name, std::string email, std::string password)
: user (username, email, password), _name{ name } {
}
student::student (std::string id) : user{ id } {
    get ();
}
bool student::add_to_database (SQLite::Database& db) {
    if (saved_in_db ())
        return true;

    SQLite::Statement query (
    db, "INSERT INTO Users(name, password_hash, email, faculty) VALUES(?, ?, ?, ?) RETURNING user_id");
    query.bindNoCopy (1, _username);
    query.bindNoCopy (2, _password_hash);
    query.bindNoCopy (3, _email);
    query.bindNoCopy (4, _faculty);

    query.executeStep ();
    int user_id = query.getColumn (0);

    std::string student_id = "23-101287";

    query = SQLite::Statement (db, "INSERT INTO Students(student_id, user_id) VALUES(?, ?)");
    query.bind (1, student_id);
    query.bind (2, user_id);

    int success = query.exec ();
    _id         = student_id;
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
    return true;
}
bool student::update_in_database (SQLite::Database& db,
std::map<std::string, std::any> props) {
    return true;
}


void student::get () {
    if (!saved_in_db ())
        throw utils::custom_exception{ "Item not saved in database;" };

    std::string query_string =
    "select users.*, students.id from students join users on "
    "users.id == students.user_id whre students.id = ?";

    SQLite::Statement query (db::database::get_db (), query_string);
    query.bind (1, _id);

    while (query.executeStep ()) {
        _password_hash = (std::string)query.getColumn (1);
        _email         = (std::string)query.getColumn (2);
        _faculty       = (std::string)query.getColumn (3);
        _name          = (std::string)query.getColumn (4);

        std::cout << "Student: " << _name << "\nID: " << _id
                  << "\nEmail: " << _email << "\nFaculty: " << _faculty << std::endl
                  << std::endl;
    }
}

} // namespace auth
