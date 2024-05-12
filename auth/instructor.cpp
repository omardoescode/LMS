#include "auth/instructor.h"

#include "utils/split_string.h"

#include <iostream>

namespace auth {

instructor::instructor (std::string id) : user (id) {
    get ();
}

instructor::instructor (std::string username, std::string email, std::string password_hash)
: user (username, email, password_hash) {
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
        _is_teaching_assistant  = query.getColumn (6).getInt ();
        std::string courses_ids = query.getColumn (7);
        _courses                = utils::split_string (courses_ids, ',');

#if DEBUGGING
        std::cout << "Instructor: " << _name << "\nID: " << _id
                  << "\nEmail: " << _email << "\nFaculty: " << _faculty
                  << "\nTeaching Assistant: " << (_is_teaching_assistant ? "Yes" : "No")
                  << "\nCourses: " << courses_ids << std::endl
                  << std::endl;
#endif
    }
}
bool instructor::add_to_database (SQLite::Database& db) {
    return true;
}
bool instructor::remove_from_database (SQLite::Database& db) {
    return true;
}
bool instructor::update_in_database (SQLite::Database& db,
std::map<std::string, std::any> props) {
    return true;
}
} // namespace auth
