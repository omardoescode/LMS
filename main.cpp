#include "auth/administrator.h"
#include "auth/instructor.h"
#include "auth/student.h"
#include "db/database.h"
#include <iostream>
auto main () -> int {
    // db::database::get_instance ().refresh_and_seed_db ();
    /*int id;
    std::cout << "Course ID: ";
    std::cin >> id;
    learn::course c (std::to_string (id));
    std::string stud_id;
    std::cout << "Student ID: ";
    std::cin >> stud_id;
    auth::student stud (stud_id);
    std::string inst_id;
    std::cout << "Instructor ID: ";
    std::cin >> inst_id;
    auth::instructor ins (inst_id);
    std::string admin_id;
    std::cout << "Admin ID: ";
    std::cin >> admin_id;
    auth::administrator admin (admin_id);*/
    int a_id;
    std::cout << "Assignment ID: ";
    std::cin >> a_id;
    learn::assignment a (std::to_string (a_id));
    int s_id;
    std::cout << "Assignment Submission ID: ";
    std::cin >> s_id;
    learn::assignment_submission s (std::to_string (s_id));
}
