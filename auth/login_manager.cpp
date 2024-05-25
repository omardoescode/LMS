#include "auth/login_manager.h"
#include "auth/administrator.h"
#include "auth/instructor.h"
#include "auth/session.h"
#include "auth/student.h"
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
    case user::Role::ADMINISTRATOR: return std::make_unique<administrator> (id);
    case user::Role::INSTRUCTOR: return std::make_unique<instructor> (id);
    case user::Role::STUDENT: return std::make_unique<student> (id);
    }
}

std::unique_ptr<user>
login_manager::load_user_by_user_id (std::string user_id, user::Role role) {
    switch (role) {
    case user::Role::ADMINISTRATOR: {
        utils::vector<std::unique_ptr<administrator>> results =
        administrator::getAdministrators ({ { "users.id"s, user_id } });
        if (results.empty ())
            throw utils::custom_exception{ "Invalid user_id" };
        return std::move (results[0]);
    }
    case user::Role::INSTRUCTOR: {
        utils::vector<std::unique_ptr<instructor>> results =
        instructor::getInstructors ({ { "users.id"s, user_id } });
        if (results.empty ())
            throw utils::custom_exception{ "Invalid user_id" };
        return std::move (results[0]);
    }
    case user::Role::STUDENT: {
        utils::vector<std::unique_ptr<student>> results =
        student::getStudents ({ { "users.id"s, user_id } });
        if (results.empty ())
            throw utils::custom_exception{ "Invalid user_id" };
        return std::move (results[0]);
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

            std::unique_ptr<user> user =
            load_user (user_id, user::string_to_role (role_string));
            _sessions.push_back (session (std::move (user), time));
            infile.close ();
        }
    } catch (const std::filesystem::filesystem_error& ex) {
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
        return false;
    }


    auth::user::Role _role = auth::user::string_to_role (query.getColumn (5));
    _current_user          = load_user_by_user_id (user_id, _role);

    // Check a password
    if (!_current_user->is_correct_password (password))
        return false;

    // Create a session for the user
    auth::session session (_current_user, time (NULL));
    session.save_session ();

    return true;
}

bool login_manager::login_by_id (std::string id, std::string password) {
    std::unique_ptr<auth::user> res = nullptr;
    auto students_trial = auth::student::getStudents ({ { "students.id"s, id } });
    if (!students_trial.empty ())
        res = std::move (students_trial[0]);


    if (!res) {
        auto instructors_trial =
        auth::instructor::getInstructors ({ { "instructors.id"s, id } });
        if (!instructors_trial.empty ())
            res = std::move (instructors_trial[0]);
    }

    if (!res) {
        auto admins_trial =
        auth::administrator::getAdministrators ({ { "administrators.id"s, id } });
        if (!admins_trial.empty ())
            res = std::move (admins_trial[0]);
    }

    if (!res || !res->is_correct_password (password)) {
        return false;
    }

    _current_user = std::move (res);
    auth::session session (_current_user, time (NULL));
    session.save_session ();
    return true;
}

bool login_manager::login (int session_index) {
    if (!_sessions.valid_index (session_index))
        return false;
    auto session = _sessions[session_index];

    if (session.has_expired ())
        return false;

    _current_user          = session.get_user ();
    _current_session_index = session_index;

    return true;
}

bool login_manager::logout () {
    if (!is_logged ())
        return false;

    _current_session_index = -1;
    _current_user          = nullptr;

    std::cout << "made it here" << std::endl;
    return true;
}
} // namespace auth
