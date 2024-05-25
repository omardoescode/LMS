#pragma once
#include "db/database.h"
#include "db/database_item.h"
#include "utils/exceptions.h"
#include "utils/vector.h"
#include <iostream>
#include <string>

namespace auth {
class user : public db::database_item {
public:
    enum class Role { ADMINISTRATOR, INSTRUCTOR, STUDENT };
    // Consturctor
    // Will be used for construction of the subclasses
    // Simply assigning parameters to properties of the object using member
    // initializer list
    user (std::string name, std::string faculty, std::string email, std::string password, Role role);

    // Intialize as well
    // Used for subclasses
    explicit user (std::string id) : database_item (id){};

    // Getters
    std::string get_username () const {
        return _id;
    }
    std::string get_email () const {
        return _email;
    }
    std::string get_name () const {
        return _name;
    }
    Role get_role () const {
        return _role;
    }
    std::string get_user_id () const {
        return _user_id;
    }

    static std::string role_to_string (Role role);
    static Role string_to_role (std::string role);


    // Setters
    // When applied, we need to update in DB as well
    bool set_email (std::string new_email);
    // Helpful Functions
    bool is_correct_password (std::string password) const {
        std::hash<std::string> hasher;
        std::string hashed = std::to_string (hasher (password));
        std::cout << _password_hash << "vs. " << hashed << std::endl;

        return _password_hash == hashed;
    }

    // Overriden Functions
    virtual bool add_to_database (SQLite::Database& db)      = 0;
    virtual bool remove_from_database (SQLite::Database& db) = 0;
    virtual bool update_in_database (SQLite::Database& db,
    std::map<std::string, std::any> props)                   = 0;
    virtual void get ()                                      = 0;

protected:
    std::string _name, _password_hash, _email, _faculty, _user_id;
    Role _role;
};
} // namespace auth
