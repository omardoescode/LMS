#pragma once
#include "db/database_item.h"
#include "learn/assignment.h"
#include <list>
#include <vector>
// To prevent looping includes, instead of including it there
namespace auth {
class instructor;
class student;
} // namespace auth
namespace learn {
class course : public db::database_item {
public:
  // Getters
  int get_credit_hours() const { return _credit_hours; }
  auth::instructor get_professor() const;
  std::list<auth::instructor> get_teaching_assistants() const;
  std::list<auth::student> get_students() const;
  std::string get_textbook() const { return _textbook; }

  // Setters
  // Include saving in DB for each
  bool set_credit_hours(int new_value);
  // Helpful Functions
  // Search in database for assignments that have this course
  std::list<learn::assignment> get_assigments() const;

  // Calculations
  double get_average_grade(learn::assignment &assignment) const {
    return assignment.get_average_grade();
  }
  double get_minimum_grade(learn::assignment &assignment) const {
    return assignment.get_minimum_grade();
  }
  double get_maximum_grade(learn::assignment &assignment) const {
    return assignment.get_maximum_grade();
  }
  // Overriden Functions
  bool add_to_database(SQLite::Database& db) override;
  bool remove_from_database(SQLite::Database& db) override;
  bool update_in_database(SQLite::Database& db, std::map<std::string, std::any> props) override;

private:
  int _credit_hours;
  std::string _professor, _textbook;
  std::vector<std::string> _teaching_assistants,
      _students; // Up to 6 teaching assistants
};

} // namespace learn
