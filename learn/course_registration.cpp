#include "learn/course_registration.h"
#include "auth/student.h"
#include "db/database.h"
#include "learn/course.h"
namespace learn {
course_registration::course_registration (std::string id)
: db::database_item (id) {
    get ();
}
course_registration::course_registration (std::string course, std::string student, CourseRegistrationState state)
: _course{ course }, _student{ student }, _state{ state } {
}
bool course_registration::course_registration::set_state (CourseRegistrationState new_state) {
    _state = new_state;
    if (db::database::get_instance ().update_item (*this, { { "state", new_state } })) {
        _state = new_state;
        return true;
    }
    return false;
}

void course_registration::get () {
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

learn::course course_registration::get_course () {
    return learn::course (_course);
};

auth::student course_registration::get_student () {
    return auth::student (_student);
};
} // namespace learn
