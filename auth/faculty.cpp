#include "auth/faculty.h"

namespace auth {

bool faculty::add_to_database (SQLite::Database& db) {
    return true;
}
bool faculty::remove_from_database (SQLite::Database& db) {
    return true;
}
bool faculty::update_in_database (SQLite::Database& db,
std::map<std::string, std::any> props) {
    return true;
}
} // namespace auth
