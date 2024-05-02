#pragma once
#include "db/db.h"
#include "learning/learning.h"
#include <any>
#include <string>
#include <string_view>
#include <vector>

namespace auth {
class user : public db::database_item {
public:
  // Consturctor
  user(std::string username, std::string email,
       std::string password);     //  Will include adding it to the database
  explicit user(std::string _id); // Load from the database

  // Getters
  std::string get_username() const { return _username; }
  std::string get_email() const { return _email; }

  // Setters
  // When applied, we need to update in DB as well
  bool set_username(std::string_view);
  bool set_email(std::string_view);

  // Helpful Functions
  bool check_password(std::string password);

protected:
  std::string _id, _name, _username, _password_hash, _email, _faculty;
};

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
  // instructors
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
  std::vector<learning::course_registration> list_pending_registrations() const;

  // takes a course registration and changes its value to Enrolled
  bool register_course_for_student(learning::course_registration);

  // Given a student and a course, add this student to the course
  // by creating a course registration and saving it to the database
  // HINT: Create the course registrationon and pass it the above funciton
  bool register_course_for_student(std::string_view course_id,
                                   std::string_view student_id);

  // Given a course, add it to the same faculty as this administrator is and
  // add it to the DB
  bool add_course(learning::course);

  // Overridden functions
  bool add_to_database() override;
  bool remove_from_database() override;
  bool update_in_database(std::map<std::string, std::any> props) override;
};

class instructor : public user {
public:
  // Given a course in the database, assign this course to this instructor
  bool add_course(learning::course &course);

  // Remove the course from the list of courses taught by this instructor
  bool remove_course(learning::course &course);

private:
  // Constraint: An instructor can have up to 5 courses
  std::vector<std::string> courses;
};
class student : public user {
public:
  // Constructors
  student(std::string username, std::string name, std::string email,
          std::string password); // Create with empty ID to indicate
                                 // it hasn't been added to the database yet

  explicit student(std::string id); // Load from database Getters
  std::string get_id() const { return _id; }
  std::vector<learning::course> get_courses() const;

  // Helpful Functions
  bool saved_in_db() const { return _id != ""; }

  // Specific Functions
  bool drop_course(std::string course);
  bool register_course(std::string course);
  double show_total_grades() const;
  double get_average_grades_in_all_courses() const;

  // Overridden functions
  bool add_to_database() override;
  bool remove_from_database() override;
  bool update_in_database(std::map<std::string, std::any> props) override;

private:
  std::string _name, _id;
  std::vector<std::string> _courses_registrations;
};

} // namespace auth
