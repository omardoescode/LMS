#pragma once
#include "db/database_item.h"
#include "learn/course.h"
namespace learn {
class course_registration : public db::database_item {
public:
    enum class CourseRegistrationState {
        ENROLLED,
        DROPPED,
        AWAITING_APPROVAL,
        FINISHED
    };
    // Constructor
    course_registration (std::string course,
    std::string student,
    CourseRegistrationState state = CourseRegistrationState::AWAITING_APPROVAL);
    course_registration (std::string id);
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
    void get () override;

    // Static Functions
    static utils::vector<std::unique_ptr<course_registration>>
    getCourseRegistrations (std::map<std::string, std::string> props);
    static CourseRegistrationState enum_translate (std::string);
    static std::string enum_translate (CourseRegistrationState);

private:
    std::string _course, _student;
    CourseRegistrationState _state;
};
} // namespace learn
