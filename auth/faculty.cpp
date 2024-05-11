#include "auth/faculty.h"
#include "auth/administrator.h"
#include "db/database_item.h"

namespace auth {

faculty::faculty (std::string id) : db::database_item (id) {
    get ();
}
faculty::faculty (std::string name, std::string administrator)
: _name{ name }, _administrator{ administrator } {
}
void faculty::get () {
}
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
