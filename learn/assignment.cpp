#include "learn/assignment.h"
namespace learn {

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
} // namespace learn
