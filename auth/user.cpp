#include "auth/user.h"
#include "utils/exceptions.h"

namespace auth {
const std::hash<std::string> hasher;
user::user (std::string name, std::string faculty, std::string email, std::string password, Role role)
: _name{ name }, _faculty (faculty), _email{ email },
  _password_hash{ std::to_string (hasher (password)) }, _role (role) {
}


bool user::set_username (std::string new_username) {
    if (new_username.empty ())
        throw utils::custom_exception{ "Invalid username" };
    if (db::database::get_instance ().update_item (*this, { { "username", new_username } })) {
        _username = new_username;
        return true;
    }
    return false;
}

bool user::set_email (std::string new_email) {
    if (new_email.empty ())
        throw utils::custom_exception{ "Invalid email" };
    if (db::database::get_instance ().update_item (*this, { { "email", new_email } })) {
        _email = new_email;
        return true;
    }
    return false;
}


std::string user::role_to_string (Role role) {
    switch (role) {
    case Role::STUDENT: return "Student";
    case Role::INSTRUCTOR: return "Instructor";
    case Role::ADMINISTRATOR: return "Administrator";
    }
}

user::Role user::string_to_role (std::string role) {
    if (role == "Student")
        return Role::STUDENT;
    if (role == "Instructor")
        return Role::INSTRUCTOR;
    if (role == "Administrator")
        return Role::ADMINISTRATOR;
    throw utils::custom_exception ("Invalid role");
}

} // namespace auth
