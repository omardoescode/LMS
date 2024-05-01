// Scoped Pointer Tests
#include "../db/db.h"
#include "Tester.h"

bool test_db() {
  auto &db = db::database::get_instance();
  return true;
}

int main() {
  using namespace std::string_literals;
  Tester test;
  test(test_db, "test access for database");

  return 0;
}
