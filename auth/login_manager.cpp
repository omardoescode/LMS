#include "auth/login_manager.h"
#include "auth/administrator.h"
#include "auth/instructor.h"

namespace auth {
login_manager::login_manager ()
: _current_session_id (-1), _current_user (nullptr) {
}

std::unique_ptr<user> login_manager::load_user (std::string id, user::Role role) {
    switch (role) {
    case user::Role::ADMINISTRATOR:
        return std::unique_ptr<administrator> (new administrator (id));

    case user::Role::INSTRUCTOR:
        return std::unique_ptr<instructor> (new instructor (id));

    case user::Role::STUDENT:
        return std::unique_ptr<student> (new student (id));
    }
}
} // namespace auth
