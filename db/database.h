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

/*
 * Examples
 * db::database::get_instance().add_item(<administrator_object>);
 * db::database::get_instance().remove_item(<administrator_object>);
 * db::database::get_instance().update_item(<administrator_object>,
 * <hash_map_like{username: "omar"}>
 * );
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
  bool add_item(db::database_item &item) { return item.add_to_database(_db); }
  bool remove_item(db::database_item &item) {
    return item.remove_from_database(_db);
  }

  // Updating the object given the properties we want to update
  // Types of std::any will need to be casted using
  // std::any_cast<dest_type>(obj)
  // Cast each property in the method used to the
  // proper type in item.update_in_database
  bool update_item(db::database_item &item,
                   std::map<std::string, std::any> new_props) {
    return item.update_in_database(_db, std::move(new_props));
  }

  std::list<std::unique_ptr<database_item>>
  get(std::string table_name, std::map<std::string, std::string> &props);

  // Write more functions that are more helpful in terms of the other one
  // Template ones must be defined and declared in the same place
  // template <typename T>
  // T get_elem_by_id(std::string table_name, std::string_view _id) {
  //   std::map<std::string, std::string> props;
  //   props["_id"] = _id;
  //   auto vals = db::database::get_instance().get(table_name, props);
  //   if (vals.empty())
  //     throw utils::custom_exception{"No professor has this id"};
  //   if (auto casted = dynamic_cast<T>(vals.front()))
  //     return casted;
  //   throw utils::custom_exception{"Casting failed"};
  // }
  //
  // template <typename T>
  // T get_elems_by_id(std::string &table_name, std::vector<std::string> &ids) {
  //   std::list<T> res;
  //   for (auto &id : ids)
  //     res.push_back(
  //         db::database::get_instance().get_elem_by_id<T>(table_name, id));
  //   return res;
  // }
  //
  // template <typename T>
  // T get_elems_by_id(std::string table_name, std::list<std::string> ids) {
  //   std::list<T> res;
  //   for (auto &id : ids)
  //     res.push_back(
  //         db::database::get_instance().get_elem_by_id<T>(table_name, id));
  //   return res;
  // }

private:
  // initialize_db
  void initialize_db();

  SQLite::Database _db;
  database(); // A singleton Class
};
} // namespace db
