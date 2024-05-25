#include "auth/administrator.h"
#include "auth/instructor.h"
#include "auth/student.h"
#include <iostream>
bool test_creating_user_by_admin () {
    auth::administrator admin ("1");
    auth::student student (
    "Omar Mohammad", "Computer Science", "omarmohammad@gmail.com", "OmarReigns");
    admin.add_user (student);

    if (!student.saved_in_db ())
        return false;

    auth::student same_student (student.get_id ());


    return student.get_email () == "omarmohammad@gmail.com" &&
    student.is_correct_password ("OmarReigns");
}


bool test_creating_instructor_by_admin () {
    auth::administrator admin ("1");
    auth::student instructor (
    "Omar Mohammad", "Computer Science", "omarmohammad@gmail.com", "OmarReigns");
    admin.add_user (instructor);

    if (!instructor.saved_in_db ()) {

        std::cout << "not saved" << std::endl;
        return false;
    }
    auth::student same_instructor (instructor.get_id ());

    if (same_instructor.get_email () != "omarmohammad@gmail.com")
        return false;
    // Assign some courses to the insturcotor
    // tODO: Complete the test
    return true;
}
