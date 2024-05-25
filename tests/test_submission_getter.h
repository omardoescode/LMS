#include "learn/course.h"
#include <iostream>
bool test_submission_getter () {
    // Get the course
    learn::course course ("23");

    // Get the assignments
    std::cout << course.get_assignments ().size () << std::endl;
    return true;
}
