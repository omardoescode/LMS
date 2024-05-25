#include "db/database.h"
#include <filesystem>
int main () {
    db::database::get_instance ().refresh_and_seed_db ();

    // remove the sessions directory if any
    std::filesystem::remove_all ("./sessions");
    return 0;
}
