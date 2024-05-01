#pragma once
#include "../db/db.h"
#include "../learning/learning.h"
#include <string>
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
  std::string _id, _username, _password_hash, _email, _faculty;
};

class student : public user {
public:
  // Constructors
  student(std::string username, std::string name, std::string email,
          std::string password);    // Create and add to database
  explicit student(std::string id); // Load from database

  // Getters
  std::string get_id() const { return _id; }
  std::vector<learning::course> get_courses() const;

  // Specific Functions
  bool drop_course(std::string course);
  bool register_course(std::string course);
  double show_total_grades() const;
  double get_average_grades_in_all_courses() const;

  // Overridden functions
  bool add_to_database() override;
  bool remove_from_database() override;
  bool update_in_database(std::map<std::string, std::string> props) override;

private:
  std::string _name, _id;
  std::vector<std::string> _courses_registrations;
};

class administrator : public user {
public:
  // Constructors
  administrator(std::string username, std::string email, std::string password);
  administrator(std::string id);

  // Specific Functions
  std::vector<learning::course_registration> list_pending_registrations() const;
  bool register_course_for_student(std::string student_id,
                                   std::string course_id);

  // Overridden functions
  bool add_to_database();
  bool remove_from_database();
  bool update_in_database(std::map<std::string, std::string> props);
};

} // namespace auth
