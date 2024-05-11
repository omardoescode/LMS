#include <ctime>
#include <string>

namespace utils {
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

    std::string DateTime () {
        char buffer[90];
        struct tm* timeinfo = localtime (&time);
        strftime (buffer, sizeof (buffer), format, timeinfo);
        return buffer;
    }

    void set_time (std::string datetimestring) {
        set_time (datetimestring.c_str ());
    }
    void set_time (const char* datatimestring) {
        struct tm tmStruct;
        // strptime (datatimestring, format, &tmStruct);
        time = mktime (&tmStruct);
    }

    std::string operator() () {
        return DateTime ();
    }
};
} // namespace utils
