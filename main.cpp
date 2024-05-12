#include "auth/administrator.h"
#include "auth/instructor.h"
#include "auth/student.h"
#include <iostream>
auto main () -> int {
    std::string id;
    std::cin >> id;
    auto student = auth::student::get_by_id (id);
    std::cout << student->get_email ();
}
