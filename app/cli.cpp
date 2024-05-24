#include "app/cli.h"
#include <iostream>

namespace cli {
void say_hi () {
    std::cout << "Welcome to Athena" << std::endl;
}

int show_options () {
    std::cout << "1. Load Sessions" << std::endl;
    std::cout << "2. Login User" << std::endl;

    int option = 0;
    while (std::cin >> option && (option < 1 || option > 2))
        std::cout << "Invalid Option" << std::endl;
    return option;
}

void login_user () {
}
} // namespace cli
