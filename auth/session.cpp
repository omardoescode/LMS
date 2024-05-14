#include "auth/session.h"
#include "utils/datetime_reader.h"
#include "utils/exceptions.h"
#include <cassert>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
namespace auth {
session::session () : _session_id (-1), _user (nullptr) {
    generate_directory ();
}

session::session (int session_id, std::shared_ptr<user> user, time_t time)
: _session_id (session_id), _user (user), _time (time) {
    generate_directory ();
}

// bool session::search_sessions (std::string target_user_id, session* target_session) {
//     std::string path = std::filesystem::current_path ().string () + "/" + file;
//     std::cout << path;
//     if (!std::filesystem::exists (path))
//         return false;
//     std::ifstream infile (path);
//
//     int session_id;
//     std::string user_id, role_string, time_string;
//
//
//     while (!infile.eof ()) {
//         // read data
//         infile >> session_id >> user_id >> role_string >> time_string;
//
//         std::cout << session_id << " " << user_id << " " << role_string << " " << time_string;
//         if (user_id != target_user_id)
//             continue;
//
//         user::Role role = user::string_to_role (role_string);
//
//         std::shared_ptr<user> user =
//         login_manager::get_instance ().load_user (user_id, role);
//
//         utils::datetime_reader dtr (time_string);
//
//         *target_session = session (session_id, user, dtr.get_time ());
//
//         infile.close ();
//         return true;
//     }
//
//     return false;
// }


std::string session::generate_path (std::string id) {
    return std::filesystem::current_path ().string () + "/" +
    sessions_directory + "/" + id + ".txt";
}

bool session::search_sessions (std::string target_user_id, session* target_session) {
    std::string path = generate_path (target_user_id);
    if (!std::filesystem::exists (path))
        return false;
    std::ifstream infile (path);

    int session_id;
    std::string user_id, role_string, time_string;
    infile >> session_id >> user_id >> role_string >> time_string;


    user::Role role = user::string_to_role (role_string);

    std::shared_ptr<user> user = login_manager::get_instance ().load_user (user_id, role);

    utils::datetime_reader dtr (time_string);

    *target_session = session (session_id, user, dtr.get_time ());

    infile.close ();
    return true;
}


void session::save_session () {
    if (!valid_session ())
        throw utils::custom_exception{ "The session is invalid" };
    utils::datetime_reader dtr ("now");
    _time            = dtr.get_time ();
    std::string path = generate_path (_user->get_id ());

    std::ofstream outfile;

    outfile.open (path);
    assert (std::filesystem::exists (path));

    outfile << _session_id << std::endl;
    outfile << _user->get_id () << std::endl;
    outfile << user::role_to_string (_user->get_role ()) << std::endl;
    outfile << dtr.to_string () << std::endl;

    outfile.close ();
}

void session::update_session_time_to_now () {
}
void session::generate_directory () {
    std::string dir_path = std::filesystem::current_path ().string () + "/" + sessions_directory;
    if (!std::filesystem::exists (dir_path))
        std::filesystem::create_directory (dir_path);
}
} // namespace auth
