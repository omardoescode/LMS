#include "auth/login_manager.h"
#include "auth/administrator.h"
#include "auth/instructor.h"
#include "auth/session.h"
#include "utils/exceptions.h"
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

std::unique_ptr<user>
login_manager::load_user_by_user_id (std::string user_id, user::Role role) {
    switch (role) {
    case user::Role::ADMINISTRATOR: {
        std::string query_string =
        "SELECT administrators.* from instructors WHERE user_id = ?";
        SQLite::Statement query (db::database::get_db (), query_string);
        query.bind (1, user_id);
        if (!query.executeStep ())
            throw utils::custom_exception{ "Invalid user_id" };
        return std::unique_ptr<administrator> (
        new administrator ((std::string)query.getColumn (0)));
    }
    case user::Role::INSTRUCTOR: {
        std::string query_string =
        "SELECT instructors.* from instructors WHERE user_id = ?";
        SQLite::Statement query (db::database::get_db (), query_string);
        query.bind (1, user_id);

        if (!query.executeStep ())
            throw utils::custom_exception{ "Invalid user_id" };
        return std::unique_ptr<instructor> (
        new instructor ((std::string)query.getColumn (0)));
    }
    case user::Role::STUDENT: {
        std::string query_string =
        "SELECT students.* from students WHERE user_id = ?";
        SQLite::Statement query (db::database::get_db (), query_string);
        query.bind (1, user_id);
        if (!query.executeStep ())
            throw utils::custom_exception{ "Invalid user_id" };
        return std::unique_ptr<student> (new student ((std::string)query.getColumn (0)));
    }
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

bool login_manager::login (std::string user_id, std::string password) {
    if (is_logged ())
        throw utils::custom_exception{ "You are already logged in" };

    // Get users info from database
    std::string query_string = "SELECT users.* from users WHERE id = ?";
    SQLite::Statement query (db::database::get_db (), query_string);
    query.bind (1, user_id);

    if (!query.executeStep ()) {
        std::cout << "user_id " << user_id << " is not found in db" << std::endl;
        return false;
    }


    auth::user::Role _role = auth::user::string_to_role (query.getColumn (5));
    _current_user          = load_user_by_user_id (user_id, _role);

    // Check a password
    if (!_current_user->check_password (password))
        return false;

    // Create a session for the user
    auth::session session (_current_user, time (NULL));
    session.save_session ();

    return true;
}

bool login_manager::login (int session_id) {
    if (!_sessions.valid_index (session_id))
        return false;
    auto session = _sessions[session_id];
    // TODO: handle when the session has expired

    if (session.has_expired ())
        return false;

    _current_user          = session.get_user ();
    _current_session_index = session_id;

    return true;
}

bool login_manager::logout () {
    if (!is_logged ())
        return false;

    _current_session_index = -1;
    _current_user          = nullptr;
    return true;
}
} // namespace auth
