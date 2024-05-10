#include "learn/assignment.h"
namespace learn {

assignment::assignment (std::string id) : db::database_item (id) {
    get ();
}
assignment::assignment (std::string course, AssignmentType type)
: _course{ course }, _type{ type } {
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
