#include "auth/administrator.h"
#include "db/database.h"
#include <filesystem>
int main () {
    db::database::get_instance ().refresh_and_seed_db ();

    // remove the sessions directory if any
    std::filesystem::remove_all ("./sessions");

    // Add A special administrator
    auth::administrator admin ("Cherry Ahmed", "cherry.ahmed@eui.com",
    "Computer Science", "CherryIsTheBestDoc");
    db::database::get_instance ().add_item (admin);
    return 0;
}
