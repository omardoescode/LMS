#include "learn/assignment_submission.h"
#include "auth/student.h"
#include "utils/exceptions.h"

#include <iostream>

namespace learn {

assignment_submission::assignment_submission (std::string id)
: db::database_item (id) {
    get ();
}
assignment_submission::assignment_submission (std::string assignment, std::string student, double grade)
: _assignment{ assignment }, _student (student), _grade (grade) {
}
assignment_submission::assignment_submission (std::string assignment, std::string student)
: _assignment (assignment), _student (student), _grade{ -1 } {
    _submission_datetime = time (NULL);
}

double assignment_submission::get_grade () const {
    if (!is_graded ())
        throw utils::custom_exception{ "Assignment not graded" };
    return _grade;
}

// assignment assignment_submission::get_assignment () {
//     return assignment (_assignment);
// }

// auth::student assignment_submission::get_student () {
//     return auth::student (_student);
// }


bool assignment_submission::set_grade (double new_grade) {
    if (db::database::get_instance ().update_item (
        *this, { { "grade", std::to_string (new_grade) } })) {
        _grade = new_grade;
        return true;
    }
    return false;
}
void assignment_submission ::get () {
    if (!saved_in_db ())
        throw utils::custom_exception{ "Item not saved in database;" };

    std::string query_string =
    "select * from assignmentsubmissions where id = ?";

    SQLite::Statement query (db::database::get_db (), query_string);
    query.bind (1, _id);

    while (query.executeStep ()) {
        _grade               = query.getColumn (1);
        _submission_datetime = query.getColumn (2);
        _assignment          = (std::string)query.getColumn (3);
        _student             = (std::string)query.getColumn (4);

#if PRINT_DATA_WHEN_RETRIEVED
        std::cout << "Assignment ID: " << _assignment << "\nSubmission ID: " << _id
                  << "\nGrade: " << _grade << "\nStudent ID: " << _student << std::endl
                  << "Submission Date: " << _submission_datetime << std::endl
                  << std::endl;
#endif
    }
}

bool assignment_submission::add_to_database (SQLite::Database& db) {
    SQLite::Statement query (db,
    "INSERT INTO AssignmentSubmissions(grade, assignment_id, student_id) "
    "VALUES(?,?,?) RETURNING id, submission_date");

    query.bind (1, _grade);
    query.bindNoCopy (2, _assignment);
    query.bindNoCopy (3, _student);

    query.executeStep ();

    int assignment_submission_id   = query.getColumn (0);
    int assignment_submission_date = query.getColumn (1);

    _id                  = std::to_string (assignment_submission_id);
    _submission_datetime = assignment_submission_date;
    return (stoi (_id) != 0);
}

bool assignment_submission::remove_from_database (SQLite::Database& db) {
    SQLite::Statement query (db, "DELETE FROM AssignmentSubmissions WHERE id = ?");

    query.bindNoCopy (1, _id);

    int success = query.exec ();
    return success;
}
bool assignment_submission::update_in_database (SQLite::Database& db,
std::map<std::string, std::any> props) {
    std::string query_string = "UPDATE AssignmentSubmissions SET ";
    int count                = 0;
    for (auto const& [key, val] : props) {
        if (count > 0)
            query_string += ",";

        query_string += key + " = ";

        if (key == "grade")
            query_string += std::to_string (std::any_cast<double> (val));
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
} // namespace learn
