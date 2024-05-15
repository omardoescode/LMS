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

bool login_manager::login (std::string username, std::string password) {
    // Get users info from database
    std::string query_string = "SELECT * FROM users WHERE id = ?";
    SQLite::Statement query (db::database::get_db (), query_string);
    query.bind (1, username);

    if (!query.executeStep ())
        return false;

    // auth::user::Role _role = auth::user::string_to_role ();
    // TODO: Wait till the db is finished
    // After being retrieved, create a session for them
}

bool login_manager::login (int session_id) {
    if (!_sessions.valid_index (session_id))
        return false;
    auto session = _sessions[session_id];
    // TODO: handle when the session has expired
    // _current_user          = std::shared_ptr<user> (&session.get_user ());
    _current_user          = session.get_user ();
    _current_session_index = session_id;

    return true;
}

bool login_manager::logout () {
    if (is_logged ())
        return false;

    _current_session_index = -1;
    _current_user          = nullptr;
    return true;
}
} // namespace auth
