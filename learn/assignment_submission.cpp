#include "learn/assignment_submission.h"
#include "auth/student.h"
#include "utils/datetime_reader.h"
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
    utils::datetime_reader dtr ("now");
    _submission_datetime = dtr.get_time ();
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
        _grade = query.getColumn (1);
        _submission_datetime =
        utils::datetime_reader ((std::string)query.getColumn (2)).get_time ();
        _assignment = (std::string)query.getColumn (3);
        _student    = (std::string)query.getColumn (4);

#if DEBUGGING
        std::cout
        << "Assignment ID: " << _assignment << "\nSubmission ID: " << _id
        << "\nGrade: " << _grade << "\nStudent ID: " << _student << std::endl
        << "Submission Date: "
        << utils::datetime_reader (_submission_datetime).to_string () << std::endl;
#endif
    }
}

bool assignment_submission::add_to_database (SQLite::Database& db) {
    return true;
}

bool assignment_submission::remove_from_database (SQLite::Database& db) {
    return true;
}
bool assignment_submission::update_in_database (SQLite::Database& db,
std::map<std::string, std::any> props) {
    return true;
}
} // namespace learn
