#include "db/database.h"
int main () {
    db::database::get_instance ().refresh_and_seed_db ();
    return 0;
}
