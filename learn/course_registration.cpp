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

learn::course course_registration::get_course () const {
    auto courses = course::get ({ { "id"s, _course } });
    if (courses.empty ())
        throw utils::custom_exception{
            "Invalid course. The course seems to have been deleted"
        };
    return *courses[0];
};

auth::student course_registration::get_student () const {
    auto students = auth::student::get ({ { "id"s, _course } });
    if (students.empty ())
        throw utils::custom_exception{
            "Invalid student. The student seem to have been deleted"
        };
    return *students[0];
};
} // namespace learn
