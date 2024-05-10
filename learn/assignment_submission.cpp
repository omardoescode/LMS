#include "learn/assignment_submission.h"

namespace learn {

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
