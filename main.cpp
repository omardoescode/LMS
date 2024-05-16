#include "auth/administrator.h"
#include "auth/instructor.h"
#include "auth/session.h"
#include "auth/student.h"
#include <iostream>

auto main () -> int {
    // db::database::get_instance ().refresh_and_seed_db ();
    /*
    int id;
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
    auth::administrator admin (admin_id);

    int a_id;
    std::cout << "Assignment ID: ";
    std::cin >> a_id;
    learn::assignment a (std::to_string (a_id));
    int s_id;
    std::cout << "Assignment Submission ID: ";
    std::cin >> s_id;
    learn::assignment_submission s (std::to_string (s_id));
    int r_id;
    std::cout << "Course Registration ID: ";
    std::cin >> r_id;
    learn::course_registration r (std::to_string (r_id));
    */
    /*
    auth::student new_student ("23-101975");
    std::map<std::string, std::any> props = { { "email"s, "momo@example.com"s } };
    std::cout
    << new_student.update_in_database (db::database::get_instance ().get_db (), props);
    */
    /*
    auth::instructor new_instructor ("Mohamed.Elmosalamy");
    std::cout
    << new_instructor.remove_from_database (db::database::get_instance ().get_db ());
    */
    /*
    auth::administrator new_administrator ("Momo.Admin");
    std::cout << new_administrator.remove_from_database (
    db::database::get_instance ().get_db ());
    */
    /*
    learn::assignment assignment ("3");
    std::map<std::string, std::any> props = { { "max_grade"s, 50.6 },
        { "due_date"s, 1715540500 } };

    std::cout
    << assignment.update_in_database (db::database::get_instance ().get_db (), props);
    */
}
