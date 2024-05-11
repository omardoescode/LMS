#pragma once
#include "db/database_item.h"
#include "utils/vector.h"
#include <any>
#include <ctime>
namespace learn {
class course;
class assignment : public db::database_item {
public:
    enum class AssignmentType { PAPER, ONLINE };
    // Constructor
    assignment (std::string name,
    std::string course,
    double maximum_grade,
    AssignmentType type,
    std::string start_date,
    std::string due_date,
    std::string available_until_date);
    explicit assignment (std::string id);

    // Getters
    learn::course get_course ();
    AssignmentType get_type () const {
        return _type;
    }

    // Calculations
    double get_average_grade ();
    double get_minimum_grade ();
    double get_maximum_grade ();

    // Overridden Functions
    bool add_to_database (SQLite::Database& db) override;
    bool remove_from_database (SQLite::Database& db) override;
    bool update_in_database (SQLite::Database& db,
    std::map<std::string, std::any> props) override;

    void get () override;

private:
    std::string _name, _course;
    AssignmentType _type;
    double _maximum_grade;
    utils::vector<std::string> _submissions;
    time_t _start_date, _due_date, _available_until_date;
};
} // namespace learn
