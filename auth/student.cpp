#include "auth/student.h"
#include "learn/course_registration.h"
#include "utils/exceptions.h"
#include "utils/vector.h"
#include <iostream>


namespace auth {
student::student (std::string username, std::string name, std::string email, std::string password)
: user (username, email, password), _name{ name } {
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
utils::vector<std::unique_ptr<learn::course>> getCourses () {
    // Todo
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
        throw utils::custom_exception {
            "Course not registered"
        }
    }

    bool student::remove_from_database (SQLite::Database & db) {
        return true;
    }
    bool student::update_in_database (
    SQLite::Database & db, std::map<std::string, std::any> props) {
        return true;
    }
} // namespace auth