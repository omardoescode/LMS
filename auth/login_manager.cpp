#include "auth/login_manager.h"
#include "auth/administrator.h"
#include "auth/instructor.h"
#include "auth/session.h"
#include <fstream>
#include <iostream>
#include <memory>

namespace auth {
login_manager::login_manager ()
: _current_session_index (-1), _current_user (nullptr) {
    load_sessions ();
}

std::unique_ptr<user> login_manager::load_user (std::string id, user::Role role) {
    switch (role) {
    case user::Role::ADMINISTRATOR:
        return std::unique_ptr<administrator> (new administrator (id));

    case user::Role::INSTRUCTOR:
        return std::unique_ptr<instructor> (new instructor (id));

    case user::Role::STUDENT:
        return std::unique_ptr<student> (new student (id));
    }
}

void login_manager::load_sessions () {
    try {
        std::string user_id, role_string;
        time_t time;
        for (const std::filesystem::directory_entry& x :
        std::filesystem::directory_iterator{ session::get_directory () }) {
            std::ifstream infile (x.path ().string ());
            infile >> user_id >> role_string >> time;

            std::shared_ptr<user> user =
            load_user (user_id, user::string_to_role (role_string));
            _sessions.push_back (session (user, time));

            infile.close ();
        }
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << ex.what () << '\n';
    }
}
} // namespace auth
