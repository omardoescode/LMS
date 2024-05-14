#pragma once
#include <ctime>
#include <iostream>
#include <string>
#include <time.h>

namespace utils {

#ifdef __MINGW32__
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
#endif


class datetime_reader {
private:
    time_t _time;
    constexpr static char format[] = "%Y-%m-%d %H:%M:%S";

public:
    datetime_reader (std::string datetimestring) {
        datetime_reader (datetimestring.c_str ());
    }

    datetime_reader (const char* datetimestring) {
        if (datetimestring == "now"s) {
            _time = time (NULL);
            return;
        }
        set_time (datetimestring);
    }

    datetime_reader (time_t time) : _time (time) {
    }

    std::string to_string () {
        struct tm timeinfo;
        localtime_r (&_time, &timeinfo);
        char buffer[90];
        strftime (buffer, sizeof (buffer), format, &timeinfo);
        return buffer;
    }

    void set_time (std::string datetimestring) {
        set_time (datetimestring.c_str ());
    }
    void set_time (const char* datetimestring) {
        struct tm tm;
        strptime (datetimestring, format, &tm);
        time_t t = mktime (&tm);
    }

    time_t get_time () const {
        return _time;
    }

    std::string operator() () {
        return to_string ();
    }
};
} // namespace utils
