#include "learn/assignment.h"
#include "learn/assignment_submission.h"
namespace learn {

assignment::assignment (std::string id) : db::database_item (id) {
    get ();
}
assignment::assignment (std::string name,
std::string course,
double maximum_grade,
AssignmentType type,
std::string start_date,
std::string due_date,
std::string available_until_date)
: _name{ name }, _course{ course }, _maximum_grade (maximum_grade), _type{ type } {
}


// learn::course assignment::get_course () {
//     return learn::course (_course);
// }

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
    double min = _maximum_grade;
    for (const auto& val : _submissions) {
        auto submission = assignment_submission (val);
        if (submission.is_graded ())
            min = std::min (min, submission.get_grade ());
    }
    return min;
}
bool assignment::add_to_database (SQLite::Database& db) {
    return true;
}

bool assignment::remove_from_database (SQLite::Database& db) {
    return true;
}
bool assignment::update_in_database (SQLite::Database& db,
std::map<std::string, std::any> props) {
    return true;
}
void assignment ::get () {
}
} // namespace learn
