#pragma once
#include "db/database_item.h"
#include "learn/assignment.h"
#include "utils/vector.h"
#include <list>
#include <memory>
// To prevent looping includes, instead of including it there
namespace auth {
class instructor;
class student;
} // namespace auth
namespace learn {
class course_registration;
class assignment_submission;
class course : public db::database_item {
public:
    // Constructors
    // Retirves from db
    explicit course (std::string);

    // Create a new one before saving
    course (std::string, std::string, std::string, int, std::string);

    // Getters
    int get_credit_hours () const {
        return _credit_hours;
    }
    std::string get_name () const {
        return _name;
    }
    auth::instructor get_professor ();
    utils::vector<std::unique_ptr<auth::instructor>> get_teaching_assistants ();
    utils::vector<std::unique_ptr<auth::student>> get_students ();
    std::string get_textbook () const {
        return _textbook;
    }
    utils::vector<std::unique_ptr<learn::course_registration>> get_registrations ();

    // Setters
    // Include saving in DB for each
    bool set_credit_hours (int new_value);
    // Helpful Functions
    // Search in database for assignments that have this course
    utils::vector<std::unique_ptr<learn::assignment>> get_assignments ();
    bool has_teaching_assistant (const auth::instructor& TA) const;
    // double get_total_grades ();

    // Calculations
    double get_average_grade (learn::assignment& assignment) const {
        return assignment.get_average_grade ();
    }
    double get_minimum_grade (learn::assignment& assignment) const {
        return assignment.get_minimum_grade ();
    }
    double get_maximum_grade (learn::assignment& assignment) const {
        return assignment.get_maximum_grade ();
    }
    // Overriden Functions
    bool add_to_database (SQLite::Database& db) override;
    bool remove_from_database (SQLite::Database& db) override;
    bool update_in_database (SQLite::Database& db,
    std::map<std::string, std::any> props) override;

    // Getter from db
    void get () override;
    static utils::vector<std::unique_ptr<course>> getCourses (
    std::map<std::string, std::string> props);


private:
    int _credit_hours;
    std::string _name, _professor, _textbook, _course_code;
    utils::vector<std::string> _teaching_assistants, _students, _assignments,
    _registrations; // Up to 6 teaching assistants
};

} // namespace learn
