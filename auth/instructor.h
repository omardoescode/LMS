#pragma once
#include "auth/user.h"
#include "learn/course.h"
namespace auth {

class instructor : public user {
public:
  // Given a course in the database, assign this course to this instructor
  bool add_course(learn::course &course);

  // Remove the course from the list of courses taught by this instructor
  bool remove_course(learn::course &course);

private:
  // Constraint: An instructor can have up to 5 courses
  std::vector<std::string> courses;
};
} // namespace auth
