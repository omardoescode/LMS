#include "auth/administrator.h"
#include "auth/instructor.h"
#include "auth/login_manager.h"
#include "auth/student.h"
#include <iostream>
#include <ostream>
bool test_login_by_username_student_version () {

    // Create the student
    auth::student student ("Omar mohammad", "Computer Science", "foo@bar.com", "OmarReigns");

    // Save Student
    if (!db::database::get_instance ().add_item (student)) {
        return false;
    }

    // Login by User
    auto& lg = auth::login_manager::get_instance ();
    lg.logout (); // In case of any
    // std::cout << student.get_user_id () << std::endl;
    lg.login (student.get_user_id (), "OmarRegins");

    // Check for current_user
    return lg.get_current_user () != nullptr &&
    lg.get_current_user ()->get_email () == student.get_email ();
}

bool test_login_by_username_instructor_version () {

    // Create the student
    auth::instructor instructor (
    "Mohammad Mosalamy"s, "Computer Science"s, "foo@bar.com"s, "MohaIsCool"s);

    // Save Student
    if (!db::database::get_instance ().add_item (instructor)) {
        return false;
    }

    // Login by User
    auto& lg = auth::login_manager::get_instance ();
    lg.logout (); // In case of any
    // std::cout << student.get_user_id () << std::endl;
    lg.login (instructor.get_user_id (), "MohaIsCool");

    // Check for current_user
    return lg.get_current_user () != nullptr &&
    lg.get_current_user ()->get_email () == instructor.get_email ();
}


bool test_login_by_username_administrator_version () {
    // Create the student
    auth::administrator administrator (
    "Mohammad Mosalamy", "Computer Science", "foo@bar.com", "MohaIsCool");

    // Save Student
    if (!db::database::get_instance ().add_item (administrator)) {
        return false;
    }

    // Login by User
    auto& lg = auth::login_manager::get_instance ();
    lg.logout (); // In case of any
    // std::cout << student.get_user_id () << std::endl;
    lg.login (administrator.get_user_id (), "MohaIsCool");

    // Check for current_user
    return lg.get_current_user () != nullptr &&
    lg.get_current_user ()->get_email () == administrator.get_email ();
}
bool test_logout () {
    auto& lg = auth::login_manager::get_instance ();
    lg.logout ();
    return !lg.is_logged ();
}
