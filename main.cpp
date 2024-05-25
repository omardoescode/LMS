#include "auth/login_manager.h"
#include <app/cli.h>
int main () {
    auto& lg = auth::login_manager::get_instance ();
    cli::say_hi ();
    cli::handle_login ();
    cli::handle_login_successful ();


    std::cout << "Cya soon" << std::endl;
    return 0;
}
