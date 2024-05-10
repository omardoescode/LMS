#pragma once
#include "SQLiteCpp/Database.h"
#include "auth/role.h"
#include "db/database.h"
#include "db/database_item.h"

namespace auth {
class user : public db::database_item {
public:
    // Consturctor
    // Will be used for construction of the subclasses
    // Simply assigning parameters to properties of the object using member
    // initializer list
    user (std::string username, std::string email, std::string password);

    // Intialize as well
    // Used for subclasses
    explicit user (std::string_view id) : database_item (id){};

    // Getters
    std::string get_username () const {
        return _username;
    }
    std::string get_email () const {
        return _email;
    }

    // Setters
    // When applied, we need to update in DB as well
    bool set_username (std::string_view new_username) {
        std::map<std::string, std::any> map;
        map["username"] = new_username;
        if (db::database::get_instance ().update_item (*this, map)) {
            _username = new_username;
            return true;
        }
        return false;
    }
    bool set_email (std::string_view new_email) {
        std::map<std::string, std::any> map;
        map["email"] = new_email;
        if (db::database::get_instance ().update_item (*this, map)) {
            _email = new_email;
            return true;
        }
        return false;
    }

    // Helpful Functions
    bool check_password (std::string password) const {
        std::hash<std::string> hasher;
        return _password_hash == std::to_string (hasher (password));
    }

    // Overriden Functions
    virtual bool add_to_database (SQLite::Database& db)      = 0;
    virtual bool remove_from_database (SQLite::Database& db) = 0;
    virtual bool update_in_database (SQLite::Database& db,
    std::map<std::string, std::any> props)                   = 0;

protected:
    std::string _name, _username, _password_hash, _email, _faculty;
    Role _role;
};
} // namespace auth
