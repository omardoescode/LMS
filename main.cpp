#include "auth/student.h"
#include "db/database.h"
#include <iostream>
auto main () -> int {
    // db::database::get_instance ().refresh_and_seed_db ();
    using std::string_literals::operator""s;
        std::map<std::string, std::any> map = { { "username"s, "23-101084"s } };
    auth::student::get (map);
}
