#include "app/cli.h"
#include <iostream>

namespace cli {
void say_hi () {
    std::cout << "Welcome to Athena" << std::endl;
}

int choose_user_option () {
    std::cout << "Please choose an option to log in: " << std::endl;
    std::cout << "1. Administrator" << std::endl;
    std::cout << "2. Instructor" << std::endl;
    std::cout << "3. Student" << std::endl;

    int option = 0;
    while (std::cin >> option && option != 0) {
        std::cout << "Please enter a valid option" << std::endl;
    }

    return option;
}

void login_user () {
}
} // namespace cli
