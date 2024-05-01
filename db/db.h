#pragma once
#include <map>
#include <string>
namespace db {
class database_item {
  virtual bool add_to_database() = 0;
  virtual bool remove_from_database() = 0;
  virtual bool update_in_database(std::map<std::string, std::string> props) = 0;

public:
  friend class database;
  virtual ~database_item() = default;
};

class database {
public:
  // Default operations for singletons
  database(database &) = delete;
  void operator=(const database &) = delete;

  // The only way to get through database is through this function
  static database &get_instance() {
    static database instance;
    return instance;
  }

  bool remove_item(database_item &item) { return item.remove_from_database(); }
  bool add_item(database_item &item) { return item.add_to_database(); }
  bool update_item(database_item &item,
                   std::map<std::string, std::string> new_props) {
    return item.update_in_database(std::move(new_props));
  }

private:
  // initialize_db
  void initialize_db(std::string connection_key);

  std::string _connection_key;
  database(); // A singleton Class
};
} // namespace db
