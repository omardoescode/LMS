#include "auth/student.h"
#include "db/database.h"
#include <iostream>
auto main () -> int {
    using std::string_literals::operator""s;
    // db::database::get_instance ();
    std::map<std::string, std::any> map = {};
    auth::student::get (map);
}
