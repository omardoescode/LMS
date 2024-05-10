#include "learn/course_registration.h"
#include "auth/student.h"
#include "db/database.h"
#include "learn/course.h"
#include "utils/exceptions.h"
namespace learn {
bool course_registration::course_registration::set_state (CourseRegistrationState new_state) {
    _state = new_state;
    if (db::database::get_instance ().update_item (*this, { { "state", new_state } })) {
        _state = new_state;
        return true;
    }
    return false;
}

bool course_registration::add_to_database (SQLite::Database& db) {
    return true;
}
bool course_registration::remove_from_database (SQLite::Database& db) {
    return true;
}
bool course_registration::update_in_database (SQLite::Database& db,
std::map<std::string, std::any> props) {
    return true;
}

// learn::course course_registration::get_course () {
//     auto courses = course::get ({ { "id"s, _course } });
//     if (courses.empty ())
//         throw utils::custom_exception (
//         "Invalid course. The course seems to have been deleted");
//     return *courses.front ();
// };
//
// auth::student course_registration::get_student () {
//     auto students = auth::student::get ({ { "id"s, _course } });
//     if (students.empty ())
//         throw utils::custom_exception{
//             "Invalid student. The student seem to have been deleted"
//         };
//     return *students.front ();
// };
} // namespace learn
