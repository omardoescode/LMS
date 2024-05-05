#pragma once
#include "auth/user.h"
#include "learn/course.h"
#include <vector>
namespace auth {
class student : public user {
public:
  // Constructors
  student(std::string username, std::string name, std::string email,
          std::string password); // Create with empty ID to indicate
                                 // it hasn't been added to the database yet

  explicit student(std::string id); // Load from database Getters
  std::string get_id() const { return _id; }
  std::vector<std::unique_ptr<learn::course>> get_courses() const;

  // Specific Functions
  // If the course is not registered, raise utils::custom_exceptions{"Course not
  // registered"} Otherwise, drop the course and save this to the database
  bool drop_course(std::string course);

  // If the course is registered already with any state except the Dropped
  // state, do nothing If with the dropped state, change it to Awaiting Approval
  // If the course registration is not there at all, create one with the
  // AwaitintApproval state and save it in DB
  bool register_course(std::string course);

  // Sum up all the grades in the assignment
  // If there are more than one submission for an assignment by this student,
  // choose the highest
  double show_total_grades(learn::course &course) const;

  // the average of showing total grades for all courses, using the above
  // function
  double get_average_grades_in_all_courses() const;

  // Overridden functions
  bool add_to_database() override; // The studnet is saved using his college id
  bool remove_from_database() override;
  bool update_in_database(std::map<std::string, std::any> props) override;

private:
  std::string _name;
  std::vector<std::string> _courses_registrations;
};
} // namespace auth
