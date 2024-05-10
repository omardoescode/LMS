#include "learn/course_registration.h"
#include "db/database.h"
namespace learn {
bool course_registration::course_registration::set_state (CourseRegistrationState new_state) {
    _state = new_state;
    if (db::database::get_instance ().update_item (*this, { { "state", new_state } })) {
        _state = new_state;
        return true;
    }
    return false;
}
} // namespace learn
