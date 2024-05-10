#include "auth/instructor.h"

namespace auth {
bool instructor::add_to_database (SQLite::Database& db) {
    return true;
}
bool instructor::remove_from_database (SQLite::Database& db) {
    return true;
}
bool instructor::update_in_database (SQLite::Database& db,
std::map<std::string, std::any> props) {
    return true;
}
} // namespace auth
