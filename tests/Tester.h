#include <iostream>
#include <string>

class Tester {
public:
  Tester() { count = 0; }
  void operator()(bool (*func)(), std::string_view test_name,
                  std::string_view wrong_message) {
    count++;
    if (!func()) {
      std::cerr << test_name << ": " << wrong_message << std::endl;
      return;
    }

    std::cout << "Test #" << count << ": '" << test_name << "' succeeded"
              << std::endl;
  }

  void operator()(bool (*func)(), std::string_view test_name) {
    operator()(func, test_name,
               "Testcase #" + std::to_string(count) + " failed");
  }

  int getTestsCount() const { return count; }

private:
  int count;
};
