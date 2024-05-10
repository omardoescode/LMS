#include "learn/assignment_submission.h"
#include "auth/student.h"
#include "utils/datetime_reader.h"
#include "utils/exceptions.h"

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
    _submission_datetime = dtr.DateTime ();
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
void assignment_submission::get () {
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
