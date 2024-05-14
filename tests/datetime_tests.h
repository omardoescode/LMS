#include "utils/datetime_reader.h"
#include <cctype>
bool test_datetime () {
    utils::datetime_reader dtr ("now");
    std::cout << "Time is :" << dtr.to_string () << "? [y/n]";
    char option;
    std::cin >> option;
    option = tolower (option);
    return option == 'y';
}
