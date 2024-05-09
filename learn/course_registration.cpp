#include "course_registration.h"
namespace learn {
course_registration::course_registration (std::string course, std::string student)
: _course{ course }, _student{ student },
  _state{ CourseRegistrationState::AWAITING_APPROVAL } {
}
} // namespace learn
