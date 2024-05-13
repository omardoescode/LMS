#pragma once
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>

namespace utils {

extern "C" inline char* strptime (const char* s, const char* f, struct tm* tm) {
    // Isn't the C++ standard lib nice? std::get_time is defined such that its
    // format parameters are the exact same as strptime. Of course, we have to
    // create a string stream first, and imbue it with the current C locale, and
    // we also have to make sure we return the right things if it fails, or
    // if it succeeds, but this is still far simpler an implementation than any
    // of the versions in any of the C standard libraries.
    std::istringstream input (s);
    input.imbue (std::locale (setlocale (LC_ALL, nullptr)));
    input >> std::get_time (tm, f);
    if (input.fail ()) {
        return nullptr;
    }
    return (char*)(s + input.tellg ());
}


class datetime_reader {
private:
    time_t time;
    constexpr static char format[] = "%Y-%m-%d %H:%M:%S";

public:
    datetime_reader (std::string datetimestring) {
        datetime_reader (datetimestring.c_str ());
    }

    datetime_reader (const char* datetimestring) {
        if (datetimestring == "now"s) {
            std::time_t t = std::time (0);
            std::tm* now  = std::localtime (&t);
            set_time (std::to_string (now->tm_mon + 1) + '-' +
            std::to_string (now->tm_mon + 1) + "-" + std::to_string (now->tm_mday));
            return;
        }
        set_time (datetimestring);
    }

    datetime_reader (time_t time) : time (time) {
    }

    std::string DateTime () {
        char* buffer        = new char[90];
        struct tm* timeinfo = localtime (&time);
        strftime (buffer, sizeof (buffer), format, timeinfo);
        return buffer;
    }

    void set_time (std::string datetimestring) {
        set_time (datetimestring.c_str ());
    }
    void set_time (const char* datatimestring) {
        struct tm tmStruct;
        strptime (datatimestring, format, &tmStruct);
        time = mktime (&tmStruct);
    }

    time_t get_time () const {
        return time;
    }

    std::string operator() () {
        return DateTime ();
    }
};
} // namespace utils
