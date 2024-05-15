// Scoped Pointer Tests
#include "tests/Tester.h"
#include <db/database.h>
#include <string_view>
#include <tests/sessions_tests.h>
#include <tests/utils_tests.h>


int main () {
    using namespace std::string_literals;
    Tester test;
    test (test_vector, "testing utils::vector");
    test (test_sessions, "testing auth::sessions");
    test (test_sessions_getter, "testing auth::sessions getter");

    return 0;
}
