#pragma once
#include "auth/user.h"
#include "learn/course.h"
#include "learn/course_registration.h"
namespace auth {

class administrator : public user {
public:
  // Constructors
  administrator(
      std::string_view username, std::string_view email,
      std::string_view password); // auto added to database after initialization
  administrator(std::string id);

  // Specific Functions
  // This is the only way to add students and instructors to the database
  // Invariant: this function is intended to work with only students and
  // instructors, not with administrators
  bool add_user(user &user) {
    return db::database::get_instance().add_item(user);
  }

  // This is the only way to remove students and instructors to the database
  // Invariant: this function is intended to work with only students and
  // instructors
  bool remove_user(user &user) {
    return db::database::get_instance().remove_item(user);
  }

  // lists all the course registrations for all the students in
  // the same faculty with type AwaitingApproval
  std::vector<learn::course_registration> list_pending_registrations() const;

  // takes a course registration and changes its value to Enrolled
  bool register_course_for_student(learn::course_registration &);

  // Given a student and a course, add this student to the course
  // by creating a course registration and saving it to the database
  // HINT: Create the course registrationon and pass it the above funciton
  bool register_course_for_student(std::string_view course_id,
                                   std::string_view student_id);

  // Given a course, add it to the same faculty as this administrator is and
  // add it to the DB
  bool add_course(learn::course &);

  // Overridden functions
  bool add_to_database() override;
  bool remove_from_database() override;
  bool update_in_database(std::map<std::string, std::any> props) override;
};
} // namespace auth
