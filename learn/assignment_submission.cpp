#include "learn/assignment_submission.h"

namespace learn {

assignment_submission::assignment_submission (std::string id)
: db::database_item (id) {
    get ();
}
assignment_submission::assignment_submission (std::string assignment, std::string student, double grade)
: _assignment{ assignment }, _student (student), _grade (grade) {
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
