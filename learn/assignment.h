#pragma once
#include "db/database_item.h"
#include <memory>
#include <vector>
namespace learn {
enum class AssigmentType { PAPER, ONLINE };
class course;
class assignment_submission;

class assignment : public db::database_item {
public:
  // Getters
  learn::course get_course() const;
  AssigmentType get_type() const { return _type; }

  // Search in DB and get the submission of this assignment
  std::vector<std::unique_ptr<learn::assignment_submission>>
  get_submission() const;

  // Calculations
  double get_average_grade() const;
  double get_minimum_grade() const;
  double get_maximum_grade() const;

  // Overridden Functions
  bool add_to_database(SQLite::Database &db) override;
  bool remove_from_database(SQLite::Database &db) override;
  bool update_in_database(SQLite::Database &db,
                          std::map<std::string, std::any> props) override;

private:
  std::string _course;
  AssigmentType _type;
};
} // namespace learn
