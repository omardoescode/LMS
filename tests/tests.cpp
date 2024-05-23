// Scoped Pointer Tests
#include "tests/Tester.h"
#include <db/database.h>
#include <string_view>
#include <tests/sessions_tests.h>
#include <tests/test_class_getters.cpp>
#include <tests/test_creating_user_by_administrator.cpp>
#include <tests/test_login_by_session.h>
#include <tests/utils_tests.h>

int main () {
    using namespace std::string_literals;
    Tester test;
    test (test_vector, "testing utils::vector");
    test (test_vector_insert_and_erase, "testing utils::vector insert and erase");
    test (test_sessions, "testing auth::sessions");
    test (test_sessions_getter, "testing auth::sessions getter");
    test (test_login_by_session, "testing auth::login_manager login by username");
    test (test_creating_user_by_admin, "testing creating user");
    test (test_creating_instructor_by_admin, "testing creating instructor");
    test (test_administrator_getter, "testing auth::administrator::getAdministrators");
    test (test_administrator_getter_with_filter,
    "testing auth::administrator::getAdministrators with filtering props");
    test (test_student_getter, "testing auth::student::getStudents");
    test (test_student_getter_with_filter,
    "testing auth::student::getStudents with filtering props");
    return 0;
}
