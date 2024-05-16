#pragma once
#include "auth/student.h"
#include "auth/user.h"
#include "learn/assignment.h"
#include "learn/assignment_submission.h"
#include "learn/course.h"
#include "utils/vector.h"
#include <memory>
#include <string_view>
namespace auth {

class instructor : public user {
public:
    // Constructors
    explicit instructor (std::string id);
    instructor (std::string name,
    std::string email,
    std::string faculty,
    std::string password,
    bool is_teaching_assistant);
    // Given a course in the database, assign this course to this instructor
    bool add_course (learn::course& course);
    bool add_course (std::string course_id);

    // Remove the course from the list of courses taught by this instructor
    bool remove_course (learn::course& course);
    bool remove_course (std::string course_id);

    // Grades Mofication

    // If not saved in db, raise utils::custom_exception{"Submission Not Saved"}
    // Otherwise, modify the grade
    bool modify_grade (learn::assignment_submission& submission, double grade);

    // If the submission is found in db, modify it using the above function
    // If not, create it and save it to the database with this grade
    bool modify_grade (auth::student& student, learn::assignment& assignment, double grade);

    // Calculations
    double get_average_grade (learn::course& course, learn::assignment& assignment) const {
        return course.get_average_grade (assignment);
    };
    double get_minimum_grade (learn::course& course, learn::assignment& assignment) const {
        return course.get_minimum_grade (assignment);
    };
    double get_maximum_grade (learn::course& course, learn::assignment& assignment) const {
        return course.get_maximum_grade (assignment);
    };

    // Members

    // If not saved, raise utils::custom_exception{"Instructor Not saved"}
    // Otherwise, add it to the list of TAs, and save this is db
    bool add_teaching_assistant (std::string course_id, auth::instructor TA);

    // If student is registered already, do nothing
    // If not, create a course registration with the state Enrolled and save
    // in the database
    bool add_student (auth::student& student, learn::course);

    // Overriden Functions
    bool add_to_database (SQLite::Database& db) override;
    bool remove_from_database (SQLite::Database& db) override;
    bool update_in_database (SQLite::Database& db,
    std::map<std::string, std::any> props) override;
    void get () override;

private:
    // Constraint: An instructor can have up to 5 courses
    bool _is_teaching_assistant;
    utils::vector<std::string> _courses;
};
} // namespace auth
