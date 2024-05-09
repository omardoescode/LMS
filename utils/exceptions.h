#pragma once
// Define a new exception class that inherits from
// std::exception
#include <exception>
#include <string>
namespace utils {
class custom_exception : public std::exception {
    private:
    std::string message;

    public:
    // Constructor accepts a const char* that is used to set
    // the exception message
    custom_exception (const char* msg) : message (msg) {
    }

    // Override the what() method to return our message
    const char* what () const throw () {
        return message.c_str ();
    }
};
} // namespace utils
