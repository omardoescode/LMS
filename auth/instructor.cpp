#include "auth/instructor.h"

namespace auth {

instructor::instructor (std::string id) : user (id) {
    get ();
}

instructor::instructor (std::string username, std::string email, std::string password_hash)
: user (username, email, password_hash) {
}
void instructor::get () {
}
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
