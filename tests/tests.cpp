// Scoped Pointer Tests
#include "tests/Tester.h"
#include <db/database.h>
#include <string_view>
#include <tests/utils_tests.h>


int main () {
    using namespace std::string_literals;
    Tester test;
    test (test_vector, "testing utils::vector");

    return 0;
}
