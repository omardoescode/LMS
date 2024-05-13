#include "auth/session.h"
#include "utils/datetime_reader.h"
#include <cassert>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
namespace auth {
session::session () : _session_id (-1), _user (nullptr) {
}

session::session (int session_id, std::shared_ptr<user> user, time_t time)
: _session_id (session_id), _user (user), _time (time), _saved (false) {
}

bool session::search_sessions (std::string target_user_id, session* target_session) {
    std::string path = std::filesystem::current_path ().string () + "/" + file;
    std::cout << path;
    if (!std::filesystem::exists (path))
        return false;
    std::ifstream infile (path);

    int session_id;
    std::string user_id, role_string, time_string;


    while (!infile.eof ()) {
        // read data
        infile >> session_id >> user_id >> role_string >> time_string;

        std::cout << session_id << " " << user_id << " " << role_string << " " << time_string;
        if (user_id != target_user_id)
            continue;

        user::Role role = user::string_to_role (role_string);

        std::shared_ptr<user> user =
        login_manager::get_instance ().load_user (user_id, role);

        utils::datetime_reader dtr (time_string);

        *target_session = session (session_id, user, dtr.get_time ());

        infile.close ();
        return true;
    }

    return false;
}

void session::save_session () {
    if (search_sessions (_user->get_id (), this))
        return;

    utils::datetime_reader dtr (_time);
    std::string path = std::filesystem::current_path ().string () + "/" + file;
    std::ofstream outfile;

    outfile.open (path, std::ios::ios_base::app);
    assert (std::filesystem::exists (path));

    outfile << _session_id << std::endl;
    outfile << _user->get_id () << std::endl;
    outfile << user::role_to_string (_user->get_role ()) << std::endl;
    outfile << dtr.DateTime () << std::endl;

    outfile.close ();
}
} // namespace auth
