#include "auth/administrator.h"

bool test_administrator_getter () {
    std::map<std::string, std::string> props = {};
    utils::vector<std::unique_ptr<auth::administrator>> restults =
    auth::administrator::getAdministrators (props);

    return restults.size () > 0;
}

bool test_administrator_getter_with_filter () {
    std::map<std::string, std::string> props = { { "users.name"s, "ADMIN 4"s } };
    utils::vector<std::unique_ptr<auth::administrator>> restults =
    auth::administrator::getAdministrators (props);

    return restults[0]->get_id () == "4";
}


bool test_student_getter () {
    std::map<std::string, std::string> props = {};
    utils::vector<std::unique_ptr<auth::student>> restults =
    auth::student::getStudents (props);

    return restults.size () > 0;
}

bool test_student_getter_with_filter () {
    auth::student student1 ("Momo", "Computer Science", "Momo_2005@gmail.com", "momotesting");
    student1.add_to_database (db::database::get_instance ().get_db ());

    std::map<std::string, std::string> props = { { "students.id"s, student1.get_id () } };
    utils::vector<std::unique_ptr<auth::student>> restults =
    auth::student::getStudents (props);

    return restults[0]->get_email () == "Momo_2005@gmail.com";
}
