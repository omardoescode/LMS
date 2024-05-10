#include "auth/administrator.h"
namespace auth {

bool administrator::add_to_database (SQLite::Database& db) {
    return true;
}
bool administrator::remove_from_database (SQLite::Database& db) {
    return true;
}
bool administrator::update_in_database (SQLite::Database& db,
std::map<std::string, std::any> props) {
    return true;
}
} // namespace auth
