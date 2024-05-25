#pragma once
#include "auth/instructor.h"
#include "auth/user.h"
#include "db/database.h"
#include "learn/course.h"
#include "learn/course_registration.h"
#include <memory>
namespace auth {

class administrator : public user {
public:
    // Constructors
    // auto added to database after initialization
    administrator (std::string name, std::string email, std::string faculty, std::string password);

    // Get from database
    // Raise utils::custom_exception("Administrator not found") if not there
    administrator (std::string id);

    // Specific Functions
    // This is the only way to add students and instructors to the database
    // Invariant: this function is intended to work with only students and
    // instructors, not with administrators
    bool add_user (user& user) {
        return db::database::get_instance ().add_item (user);
    }

    // This is the only way to remove students and instructors to the database
    // Invariant: this function is intended to work with only students and
    // instructors
    bool remove_user (user& user) {
        return db::database::get_instance ().remove_item (user);
    }

    // lists all the course registrations for all the students in
    // the same faculty with type AwaitingApproval
    utils::vector<std::unique_ptr<learn::course_registration>> list_pending_registrations ();

    // takes a course registration and changes its value to Enrolled
    bool register_course_for_student (learn::course_registration&);

    // Given a student and a course, add this student to the course
    // by creating a course registration and saving it to the database
    // HINT: Create the course registration, save it to DB, and pass it the
    // above funciton
    bool register_course_for_student (std::string course_id, std::string student_id);

    // Given a course, add it to the same faculty as this administrator is and
    // add it to the DB
    bool add_course (learn::course&);


    utils::vector<std::unique_ptr<auth::student>> get_faculty_students ();
    utils::vector<std::unique_ptr<auth::instructor>> get_faculty_instructors ();
    utils::vector<std::unique_ptr<learn::course>> get_faculty_courses ();
    bool assign_course (learn::course&, auth::instructor&);

    // Overridden functions
    bool add_to_database (SQLite::Database& db) override;
    bool remove_from_database (SQLite::Database& db) override;
    bool update_in_database (SQLite::Database& db,
    std::map<std::string, std::any> props) override;
    void get () override;
    static utils::vector<std::unique_ptr<administrator>> getAdministrators (
    std::map<std::string, std::string> props);
    // Getter from db
};
} // namespace auth
