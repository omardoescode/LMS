
#include "auth/user.h"
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
void instructor_menu ();
void student_menu ();
void logout ();
namespace administrator {
void show_students ();
void show_instructors ();
void show_courses ();
void add_student ();
void add_instructor ();
void add_course ();
void assign_course_to_instructor ();
void show_pending_requests ();
void approve_requests ();
} // namespace administrator
} // namespace cli
