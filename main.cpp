#include "auth/login_manager.h"
#include <app/cli.h>
#include <memory>
int main () {
    auto& lg = auth::login_manager::get_instance ();
    cli::say_hi ();
    int option;
    if (lg.get_sessions ().size () != 0)
        option = cli::choose_option ();
    else
        option = 2;


    switch (option) {
    case 1: cli::choose_session ();
    case 2: cli::login_user ();
    }
}
