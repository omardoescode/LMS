#include "auth/administrator.h"

#include <iostream>
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
    if (!saved_in_db ())
        throw utils::custom_exception{ "Item not saved in database;" };

    std::string query_string =
    "select users.*, administrators.id from administrators join users on "
    "users.id == administrators.user_id where administrators.id = ?";

    SQLite::Statement query (db::database::get_db (), query_string);
    query.bind (1, _id);

    while (query.executeStep ()) {
        _password_hash = (std::string)query.getColumn (1);
        _email         = (std::string)query.getColumn (2);
        _faculty       = (std::string)query.getColumn (3);
        _name          = (std::string)query.getColumn (4);

#if DEBUGGING
        std::cout << "Administrator: " << _name << "\nID: " << _id
                  << "\nEmail: " << _email << "\nFaculty: " << _faculty << std::endl
                  << std::endl;
#endif
    }
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
