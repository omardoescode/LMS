#pragma once
#include "SQLiteCpp/Backup.h"
#include "db/database_item.h"
#include "utils/exceptions.h"
#include <any>
#include <initializer_list>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace db {
class database {
public:
    // Default operations for singletons
    // Delete the copy constructor and assignment operator
    database (database&)             = delete;
    void operator= (const database&) = delete;

    // The only way to get through database is through this function
    static database& get_instance () {
        static database instance;
        return instance;
    }

    // Using Polymorphism for more clean code
    bool add_item (db::database_item& item) {
        return item.add_to_database (_db);
    }
    bool remove_item (db::database_item& item) {
        return item.remove_from_database (_db);
    }

    // Updating the object given the properties we want to update
    // Types of std::any will need to be casted using
    // std::any_cast<dest_type>(obj)
    // Cast each property in the method used to the
    // proper type in item.update_in_database
    bool update_item (db::database_item& item, std::map<std::string, std::any> new_props) {
        return item.update_in_database (_db, std::move (new_props));
    }

    std::list<std::unique_ptr<database_item>>
    get (std::string table_name, std::map<std::string, std::string>& props);

    void refresh_and_seed_db ();

    static SQLite::Database& get_db () {
        return _db;
    }

private:
    // initialize_db
    void initialize_db ();

    static SQLite::Database _db;
    database (); // A singleton Class
};
} // namespace db
