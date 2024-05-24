#include "app/cli.h"
#include "auth/login_manager.h"
#include "auth/session.h"
#include <iostream>

namespace cli {
auto& lg = auth::login_manager::get_instance ();
void say_hi () {
    std::cout << "Welcome to Athena" << std::endl;
}

int choose_option () {
    std::cout << "1. Load Sessions" << std::endl;
    std::cout << "2. Login User" << std::endl;

    int option = 0;
    std::cout << "Enter your option: ";
    while (std::cin >> option && (option < 1 || option > 2))
        std::cout << "Invalid Option" << std::endl;
    return option;
}

void choose_session () {
    utils::vector<auth::session> sessions = lg.get_sessions ();
    std::cout << sessions.size () << std::endl;
    int counter = 0;
    for (auto& session : sessions) {
        std::cout << ++counter << ". ID: " << session.get_user ()->get_id ()
                  << " NAME:" << session.get_user ()->get_name () << std::endl;
    }
    int option = 0;
    std::cout << "Enter your option: ";
    while (std::cin >> option && (option < 1 || option > sessions.size ()))
        std::cout << "Invalid option" << std::endl;

    lg.login (option - 1);
}
void login_user () {
    std::string user_id, password;
    while (true) {
        std::cout << "Enter your ID:";
        std::cin >> user_id;

        std::cout << "Enter your password";
        std::cin >> password;

        if (lg.login (user_id, password))
            break;
    }
}

void say_hi_after_login () {
    std::cout << "Welcome back, " << lg.get_current_user ()->get_name () << std::endl;
}
} // namespace cli
