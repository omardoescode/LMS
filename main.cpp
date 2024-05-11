#include "auth/student.h"
#include "db/database.h"
#include <iostream>
auto main () -> int {
    // db::database::get_instance ().refresh_and_seed_db ();
    using std::string_literals::operator""s;
    std::string name;
    std::cout << "Name: ";
    std::cin >> name;
    std::map<std::string, std::any> map = { { "name"s, name } };
    learn::course::get (map);
}
