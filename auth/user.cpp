#include "auth/user.h"

namespace auth {
const std::hash<std::string> hasher;
user::user (std::string username, std::string email, std::string password)
: _username{ username }, _email{ email },
  _password_hash{ std::to_string (hasher (password)) } {
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
} // namespace auth
