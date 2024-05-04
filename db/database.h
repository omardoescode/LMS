#pragma once
#include "db/database_item.h"
#include <any>
#include <map>
#include <string>
#include <vector>

/*
 * Examples
 * db::database::get_instance().add_item(<administrator_object>);
 * db::database::get_instance().remove_item(<administrator_object>);
 * db::database::get_instance().update_item(<administrator_object>,
 * <hash_map_like{username: "omar"}>
 * );
 *
 * When updating the value of a list, get the old list, add the new element to
 * it, and pass it back in the props map
 */
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

  // Using Polymorphism for more clean code
  bool add_item(db::database_item &item) { return item.add_to_database(); }
  bool remove_item(db::database_item &item) {
    return item.remove_from_database();
  }

  // Updating the object given the properties we want to update
  // Types of std::any will need to be casted using
  // std::any_cast<dest_type>(obj)
  // Cast each property in the method used to the
  // proper type in item.update_in_database
  bool update_item(db::database_item &item,
                   std::map<std::string, std::any> new_props) {
    return item.update_in_database(std::move(new_props));
  }

  // This function takes the name of the table and the props of the specific
  // values we want and return them from the database in a vector
  std::vector<db::database_item> get(std::string,
                                     std::map<std::string, std::string>);

private:
  // initialize_db
  void initialize_db(std::string_view connection_key);

  std::string _connection_key;
  database(); // A singleton Class
};
} // namespace db
