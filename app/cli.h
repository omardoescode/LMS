
#include "auth/user.h"
#include <cwctype>
namespace cli {
void say_hi ();
int choose_option ();
void handle_login ();
void handle_login_successful ();
void login_user ();
void choose_session ();
void say_hi_after_login ();
void show_menu ();
void administrator_menu ();
void instructors_menu ();
void students_menu ();
void logout ();
void show_courses ();
namespace administrator {
void show_students ();
void show_instructors ();
void add_student ();
void add_instructor ();
void add_course ();
void remove_student ();
void remove_instructor ();
void remove_course ();
void assign_course_to_instructor ();
void show_pending_requests ();
void approve_requests ();
} // namespace administrator
namespace instructor {
void show_students ();
void show_courses ();
void show_assignments ();
void show_maximum_grade_of_a_assignment ();
void show_minimum_grade_of_a_assignment ();
void show_average_grade_of_a_assignment ();
void create_assignment ();
void show_assignment_submission ();
void modify_assignment_submission ();
} // namespace instructor
namespace student {
void show_courses ();
void show_grades_of_course ();
void view_all_courses ();
void register_in_course ();
void drop_course ();
} // namespace student
} // namespace cli
