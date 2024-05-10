#include "auth/administrator.h"
namespace auth {

administrator::administrator (std::string username, std::string email, std::string password)
: user (username, email, password) {
}

// Get from database
// Raise utils::custom_exception("Administrator not found") if not there
administrator::administrator (std::string id) : user (id) {
    get ();
}
void administrator::get () {
}
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
