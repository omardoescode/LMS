// Scoped Pointer Tests
#include "auth/student.h"
#include "tests/Tester.h"
#include <any>
#include <db/database.h>
#include <string>
#include <string_view>
#include <tests/utils_tests.h>

// These funcitons are to be replaced once real testing begins
bool test_db() {
  auto &db = db::database::get_instance();
  return true;
}

bool test_any_cast() {
  std::any string = std::string("Hello"), integer = 5;
  if (!(std::any_cast<std::string>(string) == "Hello") ||
      !(std::any_cast<int>(integer) == 5))
    return false;

  try {
    std::any_cast<int>(string);
    return false;
  } catch (std::bad_any_cast &) {
    return true;
  }
}

bool add_student_to_db_test() {
  auth::student example = auth::student("23-101283", "Mohammad",
                                        "mohammadmosalamy@gmail.com", "Moha09");
  return db::database::get_instance().add_item(example) &&
         example.saved_in_db();
}

bool test_string_view() {
  std::string_view textv("Hello");
  return textv.substr(0, 5) == "Hello";
}

int main() {
  using namespace std::string_literals;
  Tester test;
  test(test_db, "test access for database"s);
  test(test_any_cast, "test std::any casting"s);
  test(test_string_view, "test std::string_view equality"s);
  // test(add_student_to_db_test, "test Adding a student to db"s);
  test(test_vector, "testing utils::custom_array");

  return 0;
}
