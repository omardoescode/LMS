#include "app/cli.h"
#include "auth/administrator.h"
#include "auth/instructor.h"
#include "auth/login_manager.h"
#include "auth/session.h"
#include <cstdio>
#include <iostream>

namespace cli {
auto& lg = auth::login_manager::get_instance ();
void say_hi () {
    std::cout << "Welcome to Athena" << std::endl;
}

int choose_option () {
    std::cout << "1. Load Sessions" << std::endl;
    std::cout << "2. Login User" << std::endl;
    std::cout << "3. Quit" << std::endl;

    int option = 0;
    std::cout << "Enter your option: ";
    while (std::cin >> option && (option < 1 || option > 3))
        std::cout << "Invalid Option" << std::endl;
    return option;
}
void handle_login () {
    int option;
    if (lg.get_sessions ().size () != 0)
        option = cli::choose_option ();
    else
        option = 2;


    switch (option) {
    case 1: cli::choose_session (); break;
    case 2: cli::login_user (); break;
    }
}
void handle_login_successful () {
    if (!lg.is_logged ())
        return;
    cli::say_hi_after_login ();
    cli::show_menu ();

    switch (lg.get_current_user ()->get_role ()) {
    case auth::user::Role::ADMINISTRATOR: cli::administrator_menu ();
    case auth::user::Role::INSTRUCTOR: cli::instructor_menu ();
    case auth::user::Role::STUDENT: cli::student_menu ();
    }
}
void choose_session () {
    utils::vector<auth::session> sessions = lg.get_sessions ();
    // std::cout << sessions.size () << std::endl;
    int counter = 0;
    for (int i = 0; i < sessions.size (); i++) {
        auto& session = sessions[i];
        std::cout << ++counter << ". ID: " << session.get_user ()->get_id ()
                  << " NAME:" << session.get_user ()->get_name () << std::endl;
    }
    int option = 0;
    std::cout << "Enter your option: ";
    while (std::cin >> option && (option < 1 || option > sessions.size ()))
        std::cout << "Invalid option" << std::endl;

    lg.login (option - 1);
}
void login_user () {
    std::string user_id, password;
    while (true) {
        std::cout << "Enter your ID: ";
        std::cin >> user_id;

        std::cout << "Enter your password: ";
        std::cin.ignore ();
        std::cin >> password;

        std::cout << "ID: " << user_id << std::endl;
        std::cout << "password: " << password << std::endl;
        if (lg.login_by_id (user_id, password))
            break;

        std::cout << "Invalid ID or wrong password" << std::endl;
    }
}

void say_hi_after_login () {
    std::cout << "Welcome back, " << lg.get_current_user ()->get_name () << std::endl;
}
void show_menu () {
    std::cout << "0. Quit App" << std::endl;
    std::cout << "1. Re-show menu" << std::endl;
    std::cout << "2. Logout" << std::endl;

    switch (lg.get_current_user ()->get_role ()) {
    case auth::user::Role::ADMINISTRATOR:
        std::cout << "3. Show Students" << std::endl;
        std::cout << "4. Show Instructors" << std::endl;
        std::cout << "5. Show Courses" << std::endl;
        std::cout << "6. Add Student" << std::endl;
        std::cout << "7. Add Instructor" << std::endl;
        std::cout << "8. Add Course" << std::endl;
        std::cout << "9. Assign course to instructor" << std::endl;
        std::cout << "10. Show Pending Requests" << std::endl;
        std::cout << "11. Approve Request" << std::endl;
        break;
    case auth::user::Role::INSTRUCTOR:
        std::cout << "3. Show Courses" << std::endl;
        std::cout << "4. Show Students" << std::endl;
        std::cout << "5. Show Maximum Grades of a course" << std::endl;
        std::cout << "6. Show Minimum Grades of a course" << std::endl;
        std::cout << "7. Show Average Grades of a course" << std::endl;
        std::cout << "8. Show Course Submission" << std::endl;
        std::cout << "9. Modify Course Submission" << std::endl;
        std::cout << "10. Add Course Submission" << std::endl;
        break;
    case auth::user::Role::STUDENT:
        std::cout << "3. Show Courses" << std::endl;
        std::cout << "4. Show Grades of a course" << std::endl;
        std::cout << "5. Show Unregistered Courses" << std::endl;
        std::cout << "6. Register in a course" << std::endl;
        break;
    }
}

void logout (bool& running) {
    lg.logout ();
    running = false;
    handle_login ();
    handle_login_successful ();
}

void administrator_menu () {
    bool running = true;
    while (running) {
        int option = 0;
        std::cout << "Enter your option: ";
        while (std::cin >> option && (option < 0 || option > 11))
            std::cout << "Invalid option";

        switch (option) {
        case 0: running = false; break;
        case 1: show_menu (); break;
        case 2: logout (running);
        case 3: administrator::show_students (); break;
        case 4: administrator::show_instructors (); break;
        case 5: administrator::show_courses (); break;
        case 6: administrator::add_student (); break;
        case 7: administrator::add_instructor (); break;
        case 8: administrator::add_course (); break;
        case 9: administrator::assign_course_to_instructor (); break;
        case 10: administrator::show_pending_requests (); break;
        case 11: administrator::approve_requests (); break;
        }
    }
}
void instructor_menu () {
}
void student_menu () {
}
namespace administrator {
auth::administrator& get_admin () {
    return dynamic_cast<auth::administrator&> (*lg.get_current_user ());
}
void show_students () {
    auto students = get_admin ().get_faculty_students ();
    if (students.empty ())
        std::cout << "No students in faculty yet";
    for (int i = 0; i < students.size (); i++) {
        auto& student = students[i];
        std::cout << i + 1 << ". ID: " << student->get_id ()
                  << " NAME: " << student->get_name () << std::endl;
    }
}

void show_instructors () {
    auto instructors = get_admin ().get_faculty_instructors ();
    if (instructors.empty ())
        std::cout << "No instructors in faculty yet";
    for (int i = 0; i < instructors.size (); i++) {
        auto& instructor = instructors[i];
        std::cout << i + 1 << ". ID: " << instructor->get_id ()
                  << " NAME: " << instructor->get_name () << std::endl;
    }
}
void show_courses () {
    auto courses = get_admin ().get_faculty_courses ();
    if (courses.empty ())
        std::cout << "No courses in faculty yet";
    for (int i = 0; i < courses.size (); i++) {
        auto& course = courses[i];
        std::cout << i + 1 << ". " << course->get_name () << std::endl;
    }
}
void add_student () {
    std::string name, faculty, email, password;
    std::cin.ignore ();
    std::cout << "Enter name: ";
    getline (std::cin, name);

    std::cout << "Enter faculty: ";
    getline (std::cin, faculty);


    std::cout << "Enter email: ";
    getline (std::cin, email);

    std::cout << "Enter password: ";
    getline (std::cin, password);

    auth::student student (name, faculty, email, password);
    get_admin ().add_user (student);
    std::cout << "Student added successfully" << std::endl;
}

void add_instructor () {
    std::string name, faculty, email, password, is_teaching_assistant;
    std::cin.ignore ();
    std::cout << "Enter name: ";
    getline (std::cin, name);

    std::cout << "Enter faculty: ";
    getline (std::cin, faculty);


    std::cout << "Enter email: ";
    getline (std::cin, email);

    std::cout << "Enter password: ";
    getline (std::cin, password);

    std::cout << "Is they a teaching assistant? [y/n]";
    getline (std::cin, is_teaching_assistant);

    auth::instructor instructor (
    name, faculty, email, password, is_teaching_assistant == "y");
    get_admin ().add_user (instructor);
    std::cout << "Instructor added successfully" << std::endl;
}


void add_course () {
    std::string name, textbook_title, course_code;
    int professor_number, credit_hours;
    std::cin.ignore ();
    std::cout << "Enter name: ";
    getline (std::cin, name);


    std::cout << "Enter professor number in list: ";
    std::cin >> professor_number;
    std::cin.ignore ();

    std::cout << "Enter the textbook title: ";
    getline (std::cin, textbook_title);

    std::cout << "Enter the credit hours";
    std::cin >> credit_hours;
    std::cin.ignore ();

    std::cout << "Enter the course code: ";
    getline (std::cin, course_code);

    learn::course course (name,
    get_admin ().get_faculty_instructors ()[professor_number - 1]->get_id (),
    textbook_title, credit_hours, course_code);
    get_admin ().add_course (course);
    std::cout << "Course added successfully";
}

void assign_course_to_instructor () {
    int professor_number, course_number;
    std::cout << "Enter professor number: ";
    std::cin >> professor_number;

    std::cout << "Enter course number: ";
    std::cin >> course_number;

    get_admin ().assign_course (*get_admin ().get_faculty_courses ()[course_number - 1],
    *get_admin ().get_faculty_instructors ()[professor_number - 1]);
}
void show_pending_requests () {
    auto requests = get_admin ().list_pending_registrations ();

    for (int i = 0; i < requests.size (); i++) {
        auto& request = requests[i];
        std::cout << i + 1 << ". COURSE: " << request->get_course ().get_name ()
                  << " STUDENT NAME: " << request->get_student ().get_id ()
                  << std::endl;
    }
}
void approve_requests () {

    int request_num;
    std::cout << "Enter the request number: ";
    std::cin >> request_num;

    get_admin ().register_course_for_student (
    *get_admin ().list_pending_registrations ()[request_num - 1]);
}
} // namespace administrator
} // namespace cli
