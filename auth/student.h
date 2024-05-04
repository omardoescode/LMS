#pragma once
#include "auth/user.h"
#include "learn/course.h"
namespace auth {
class student : public user {
public:
  // Constructors
  student(std::string username, std::string name, std::string email,
          std::string password); // Create with empty ID to indicate
                                 // it hasn't been added to the database yet

  explicit student(std::string id); // Load from database Getters
  std::string get_id() const { return _id; }
  std::vector<learn::course> get_courses() const;

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
