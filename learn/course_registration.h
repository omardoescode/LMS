#pragma once
#include "db/database_item.h"
#include "learn/course.h"
namespace learn {
class course_registration : public db::database_item {
    enum class CourseRegistrationState {
        ENROLLED,
        DROPPED,
        AWAITING_APPROVAL,
        FINISHED
    };

public:
    // Constructor
    course_registration (std::string id,
    std::string course,
    std::string student,
    CourseRegistrationState state = CourseRegistrationState::AWAITING_APPROVAL)
    : db::database_item (id), _course{ course }, _student{ student }, _state{ state } {
    }

    // Getters
    learn::course get_course ();
    auth::student get_student ();
    CourseRegistrationState get_state () const {
        return _state;
    }

    // Setters (save in db)
    bool set_state (CourseRegistrationState state);

    // Overridden Functions
    bool add_to_database (SQLite::Database& db) override;
    bool remove_from_database (SQLite::Database& db) override;
    bool update_in_database (SQLite::Database& db,
    std::map<std::string, std::any> props) override;

private:
    std::string _course, _student;
    CourseRegistrationState _state;
};
} // namespace learn
