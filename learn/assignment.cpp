#include "learn/assignment.h"
#include "learn/assignment_submission.h"
#include "utils/split_string.h"

#include <iostream>
namespace learn {

assignment::assignment (std::string id) : db::database_item (id) {
    get ();
}
assignment::assignment (std::string name,
std::string course,
double maximum_grade,
AssignmentType type,
time_t start_date,
time_t due_date,
time_t available_until_date)
: _name{ name }, _course{ course }, _max_grade (maximum_grade), _type{ type },
  _start_date (start_date), _due_date (due_date),
  _available_until_date (available_until_date) {
}


learn::course assignment::get_course () {
    return learn::course (_course);
}

double assignment::get_average_grade () {
    double total = 0;
    int count    = 0;
    for (const auto& val : _submissions) {
        auto submission = assignment_submission (val);
        if (submission.is_graded ())
            total += submission.get_grade (), ++count;
    }
    return count == 0 ? 0 : count / total;
}

double assignment::get_maximum_grade () {
    double max = -1;
    for (const auto& val : _submissions) {
        auto submission = assignment_submission (val);
        if (submission.is_graded ())
            max = std::max (max, submission.get_grade ());
    }
    return max;
}

double assignment::get_minimum_grade () {
    double min = _max_grade;
    for (const auto& val : _submissions) {
        auto submission = assignment_submission (val);
        if (submission.is_graded ())
            min = std::min (min, submission.get_grade ());
    }
    return min;
}
bool assignment::add_to_database (SQLite::Database& db) {
    SQLite::Statement query (db,
    "INSERT INTO Assignments(name, type, start_date, due_date, "
    "available_until_date, course_id, max_grade) VALUES(?, ?, ?, ?, ?, ?, ?) "
    "RETURNING id");

    query.bindNoCopy (1, _name);
    query.bindNoCopy (2, enum_translate (_type));
    query.bind (3, _start_date);
    query.bind (4, _due_date);
    query.bind (5, _available_until_date);
    query.bindNoCopy (6, _course);
    query.bind (7, _max_grade);

    query.executeStep ();
    int assignment_id = query.getColumn (0);

    _id = assignment_id;
    return (assignment_id != 0);
}

bool assignment::remove_from_database (SQLite::Database& db) {
    SQLite::Statement query (db, "DELETE FROM Assignments WHERE id = ?");

    query.bindNoCopy (1, _id);

    int success = query.exec ();

    return success;
}
bool assignment::update_in_database (SQLite::Database& db,
std::map<std::string, std::any> props) {
    std::string query_string = "UPDATE Assignments SET ";
    int count                = 0;
    for (auto const& [key, val] : props) {
        if (count > 0)
            query_string += ",";

        query_string += key + " = ";

        if (key == "max_grade")
            query_string += std::to_string (std::any_cast<double> (val));
        else if (key == "name" || key == "type")
            query_string += "'" + std::any_cast<std::string> (val) + "'";
        else
            query_string += std::to_string (std::any_cast<int> (val));

        count++;
    }

    query_string += " WHERE id = ?";

    SQLite::Statement query (db, query_string);
    query.bindNoCopy (1, _id);

    int success = query.exec ();
    return success;
}

assignment::AssignmentType assignment::enum_translate (std::string s) {
    switch (s == "online") {
    case true: return AssignmentType::ONLINE;
    case false: return AssignmentType::PAPER;
    }
}

std::string assignment::enum_translate (assignment::AssignmentType a) {
    switch (a == AssignmentType::ONLINE) {
    case true: return "online";
    case false: return "paper";
    }
}

utils::vector<std::unique_ptr<assignment>> assignment::getAssignments (
std::map<std::string, std::string> props) {
    std::string query_string = "select id from assignments";

    utils::vector<std::unique_ptr<assignment>> results;

    SQLite::Statement query (db::database::get_db (), query_string);

    if (!props.empty ()) {
        int ind = props.size () - 1;
        query_string += " where ";
        for (auto const& p : props) {
            query_string += p.first + " = ?" + (ind > 0 ? " and " : "");
            ind--;
        }

        query = SQLite::Statement (db::database::get_db (), query_string);
        int i = 1;

        for (auto const& p : props) {
            query.bind (i++, p.second);
        }
    }


    while (query.executeStep ()) {
        std::string assignment_id = (std::string)query.getColumn (0);
        std::unique_ptr<assignment> s (new assignment (assignment_id));
        results.push_back (std::move (s));
    }

    return results;
}

void assignment ::get () {
    if (!saved_in_db ())
        throw utils::custom_exception{ "Item not saved in database;" };

    std::string query_string =
    "select assignments.*, (select group_concat(assignmentsubmissions.id) from "
    "assignmentsubmissions where assignmentsubmissions.assignment_id = "
    "assignments.id) as submissions from assignments where assignments.id = ?";

    SQLite::Statement query (db::database::get_db (), query_string);
    query.bind (1, _id);

    while (query.executeStep ()) {
        _name = (std::string)query.getColumn (1);

        std::string type_string = (std::string)query.getColumn (2);
        _type                   = enum_translate (type_string);

        _start_date           = query.getColumn (3);
        _due_date             = query.getColumn (4);
        _available_until_date = query.getColumn (5);

        _course = (std::string)query.getColumn (6);

        _max_grade                  = query.getColumn (7);
        std::string submissions_ids = (std::string)query.getColumn (8);
        _submissions = utils::split_string (submissions_ids, ',');

#if PRINT_DATA_WHEN_RETRIEVED
        std::cout << "Assignment: " << _name << "\nID: " << _id << "\nType: " << type_string
                  << "\nStart Date: " << _start_date << "\nDue Date: " << _due_date
                  << "\nAvailable Until Date: " << _available_until_date
                  << "\nCourse ID: " << _course << "\nMax Grade: " << _max_grade
                  << "\nSubmissions: " << submissions_ids << std::endl
                  << std::endl;
#endif
    }
}
} // namespace learn
