#pragma once
#include "db/database_item.h"
#include "utils/vector.h"
#include <any>
#include <memory>
namespace learn {
class course;
class assignment_submission;

class assignment : public db::database_item {
public:
    enum class AssignmentType { PAPER, ONLINE };
    // Constructor
    assignment (std::string course, AssignmentType type);
    explicit assignment (std::string id);

    // Getters
    learn::course get_course () const;
    AssignmentType get_type () const {
        return _type;
    }

    // Search in DB and get the submission of this assignment
    utils::vector<std::unique_ptr<learn::assignment_submission>> get_submission () const;

    // Calculations
    double get_average_grade () const;
    double get_minimum_grade () const;
    double get_maximum_grade () const;

    // Overridden Functions
    bool add_to_database (SQLite::Database& db) override;
    bool remove_from_database (SQLite::Database& db) override;
    bool update_in_database (SQLite::Database& db,
    std::map<std::string, std::any> props) override;

    void get () override;

private:
    std::string _course;
    AssignmentType _type;
    utils::vector<std::string> submission;
};
} // namespace learn
