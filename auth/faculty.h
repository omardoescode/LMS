#pragma once
#include "auth/administrator.h"
#include "auth/instructor.h"
#include "auth/student.h"
#include "db/database_item.h"
#include <vector>
namespace auth {
class faculty : public db::database_item {
public:
  // Getters
  std::string get_name() const { return _name; }

  // Search in DB and return the values where faculty is this instance
  auth::administrator get_administrator() const;
  std::vector<auth::instructor> get_instructors() const;
  std::vector<auth::student> get_students() const;
  std::vector<learn::course> get_courses() const;

  // Overridden Functions
  bool add_to_database() override;
  bool remove_from_database() override;
  bool update_in_database(std::map<std::string, std::any> props) override;

private:
  std::string _name;
};
} // namespace auth
