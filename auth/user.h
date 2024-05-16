#pragma once
#include "SQLiteCpp/Database.h"
#include "db/database.h"
#include "db/database_item.h"
#include "utils/exceptions.h"
#include "utils/vector.h"

namespace auth {
class user : public db::database_item {
public:
    enum class Role { ADMINISTRATOR, INSTRUCTOR, STUDENT };
    // Consturctor
    // Will be used for construction of the subclasses
    // Simply assigning parameters to properties of the object using member
    // initializer list
    user (std::string username, std::string email, std::string password);

    // Intialize as well
    // Used for subclasses
    explicit user (std::string id) : database_item (id){};

    // Getters
    std::string get_username () const {
        return _username;
    }
    std::string get_email () const {
        return _email;
    }
    Role get_role () const {
        return _role;
    }

    static std::string role_to_string (Role role);
    static Role string_to_role (std::string role);


    // Setters
    // When applied, we need to update in DB as well
    bool set_email (std::string new_email);
    bool set_username (std::string new_username);
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
    virtual void get ()                                      = 0;

protected:
    std::string _name, _username, _password_hash, _email, _faculty;
    Role _role;
};
} // namespace auth
