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
    time_t start_date,
    time_t due_date,
    time_t available_until_date);
    explicit assignment (std::string id);

    // Getters
    std::string get_name () const {
        return _name;
    }
    learn::course get_course ();
    AssignmentType get_type () const {
        return _type;
    }

    // Calculations
    double get_average_grade ();
    double get_minimum_grade ();
    double get_maximum_grade ();
    double get_max_grade () const {
        return _max_grade;
    }

    // Overridden Functions
    bool add_to_database (SQLite::Database& db) override;
    bool remove_from_database (SQLite::Database& db) override;
    bool update_in_database (SQLite::Database& db,
    std::map<std::string, std::any> props) override;

    void get () override;

    // Static Functions
    static utils::vector<std::unique_ptr<assignment>> getAssignments (
    std::map<std::string, std::string> props);
    static AssignmentType enum_translate (std::string);
    static std::string enum_translate (AssignmentType);

private:
    std::string _name, _course;
    AssignmentType _type;
    double _max_grade = 1;
    utils::vector<std::string> _submissions;
    time_t _start_date, _due_date, _available_until_date;
};
} // namespace learn
