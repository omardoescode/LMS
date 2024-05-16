#include "auth/session.h"
#include "auth/login_manager.h"
#include "utils/exceptions.h"
#include <cassert>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
namespace auth {
session::session () : _user (nullptr) {
    generate_directory ();
}

session::session (std::shared_ptr<user> user, time_t time)
: _user (user), _time (time) {
    generate_directory ();
}

std::string session::generate_path (std::string id) {
    return std::filesystem::current_path ().string () + "/" +
    _sessions_directory + "/" + id + ".txt";
}

bool session::search_sessions (std::string target_user_id, session* target_session) {
    std::string path = generate_path (target_user_id);
    if (!std::filesystem::exists (path))
        return false;
    std::ifstream infile (path);

    std::string user_id, role_string;
    time_t time;
    getline (infile, user_id);
    getline (infile, role_string);
    infile >> time;


    user::Role role = user::string_to_role (role_string);

    std::shared_ptr<user> user = login_manager::get_instance ().load_user (user_id, role);

    *target_session = session (user, time);

    infile.close ();
    return true;
}


bool session::is_saved () const {
    auto path = generate_path (_user->get_id ());
    try {
        for (const std::filesystem::directory_entry& x : std::filesystem::directory_iterator{
             std::filesystem::current_path ().string () + "/" + _sessions_directory })
            if (x.path () == path) {
                return true;
            }
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << ex.what () << '\n';
        return false;
    }
    return false;
}
bool session::has_expired () const {
    return difftime (time (NULL), _time) > 24 * 60 * 60;
}
void session::save_session () {
    if (!valid_session ())
        throw utils::custom_exception{ "The session is invalid" };
    _time            = time (NULL);
    std::string path = generate_path (_user->get_id ());

    std::ofstream outfile;

    outfile.open (path);
    assert (std::filesystem::exists (path));

    outfile << _user->get_id () << std::endl;
    outfile << user::role_to_string (_user->get_role ()) << std::endl;
    outfile << _time << std::endl;

    outfile.close ();
}

void session::update_session_time_to_now () {
}
void session::generate_directory () {
    std::string dir_path = get_directory ();
    if (!std::filesystem::exists (dir_path))
        std::filesystem::create_directory (dir_path);
}

std::string session::get_directory () {
    return std::filesystem::current_path ().string () + "/" + _sessions_directory;
}
} // namespace auth
