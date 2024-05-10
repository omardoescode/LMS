#pragma once
#include "auth/student.h"
#include "db/database_item.h"
namespace learn {
class assignment;
class course;
class assignment_submission : public db::database_item {
public:
    // Constructors
    explicit assignment_submission (std::string id);
    assignment_submission (std::string assignment, std::string student, double grade = -1);
    // Helpful Funcitons
    bool is_graded () const {
        return _grade >= 0;
    }

    // Getters
    // Raise an exception if not graded: check using is_graded method
    double get_grade () const;

    // Getters from DB
    learn::assignment get_assignment () const;
    auth::student get_student () const;

    // Setters
    // Set & save as usual
    bool set_grade ();

    // Overridden Functions
    bool add_to_database (SQLite::Database& db) override;
    bool remove_from_database (SQLite::Database& db) override;
    bool update_in_database (SQLite::Database& db,
    std::map<std::string, std::any> props) override;
    void get () override;

private:
    std::string _assignment, _student;
    double _grade = -1; // -1 is when it's not graded yet
};
} // namespace learn
