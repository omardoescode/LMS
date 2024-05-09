#pragma once
#include "auth/administrator.h"
#include "auth/instructor.h"
#include "auth/student.h"
#include "db/database_item.h"
#include "utils/vector.h"
#include <map>
#include <memory>
namespace auth {
class faculty;
class faculty_getter {
private:
  faculty_getter() {}

public:
  static faculty_getter &get_instance() {
    static faculty_getter instance;
    return instance;
  }

  utils::vector<std::unique_ptr<faculty>> get(std::map<std::string, std::any>);

  // Negate Copying
  faculty_getter(faculty_getter &) = delete;
  void operator=(const faculty_getter &) = delete;
};
class faculty : public db::database_item {
public:
  using getter = faculty_getter;
  // Getters
  std::string get_name() const { return _name; }

  // Search in DB and return the values where faculty is this instance
  auth::administrator get_administrator() const;
  std::vector<std::unique_ptr<auth::instructor>> get_instructors() const;
  std::vector<std::unique_ptr<auth::student>> get_students() const;
  std::vector<std::unique_ptr<learn::course>> get_courses() const;

  // Overridden Functions
  bool add_to_database(SQLite::Database &db) override;
  bool remove_from_database(SQLite::Database &db) override;
  bool update_in_database(SQLite::Database &db,
                          std::map<std::string, std::any> props) override;

private:
  std::string _name;
};

} // namespace auth
