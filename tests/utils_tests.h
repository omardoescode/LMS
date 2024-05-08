#include "utils/custom_array.h"
#include "utils/exceptions.h"
#include <iostream>

bool test_custom_array() {
  std::cout << "...Initializing the custom array" << std::endl;
  utils::custom_array<int> elems = {1, 2, 3, 4, 5};
  if (elems.size() != 5 || elems.capacity() != 5)
    return false;

  std::cout << "...Testing Changes to the custom array" << std::endl;
  elems[1] = 1;
  elems[2] = 1;
  if (elems[1] != 1 || elems[2] != 1)
    return false;

  std::cout << "...Testing Pushing back" << std::endl;
  elems.push_back(10);
  if (elems.back() != 10)
    return false;

  std::cout << "...Testing Clearing" << std::endl;
  elems.clear();
  if (elems.size() != 0)
    return false;

  std::cout << "...Testing Reserving & Iterators using for-ranged loops"
            << std::endl;
  elems.reserve(10);
  if (elems.capacity() != 10)
    return false;
  for (int i = 0; i < 10; i++)
    elems.push_back(10);

  for (const auto &val : elems)
    if (val != 10)
      return false;

  std::cout << "...Testing Inserting" << std::endl;
  elems.insert(3, 5);
  if (elems[3] != 5)
    return false;

  std::cout << "...Testing Accessing an invalid index" << std::endl;
  try {
    elems[-1];
    return false;
  } catch (utils::custom_exception &) {
    try {
      elems[11];
      return false;
    } catch (utils::custom_exception &) {
      return true;
    }
  }
}
