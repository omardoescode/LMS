

#include "auth/login_manager.h"
#include "auth/student.h"
bool test_login_by_session () {
    auto& lg = auth::login_manager::get_instance ();
    lg.login (0);
    if (!lg.is_logged ())
        return false;

    return true;
}
