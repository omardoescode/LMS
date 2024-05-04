#pragma once
#include "db/database_item.h"
#include <any>
#include <map>
#include <string>
#include <vector>

namespace db {
class database {
public:
  // Default operations for singletons
  // Delete the copy constructor and assignment operator
  database(database &) = delete;
  void operator=(const database &) = delete;

  // The only way to get through database is through this function
  static database &get_instance() {
    static database instance;
    return instance;
  }

  bool remove_item(db::database_item &item) {
    return item.remove_from_database();
  }
  bool add_item(db::database_item &item) { return item.add_to_database(); }
  bool update_item(db::database_item &item,
                   std::map<std::string, std::any> new_props) {
    return item.update_in_database(std::move(new_props));
  }

  // This function takes the name of the table and the props of the specific
  // values we want and return them from the database in a vector
  std::vector<db::database_item> get(std::string,
                                     std::map<std::string, void *>);

private:
  // initialize_db
  void initialize_db(std::string_view connection_key);

  std::string _connection_key;
  database(); // A singleton Class
};
} // namespace db
