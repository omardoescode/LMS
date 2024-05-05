#pragma once
#include <any>
#include <map>
#include <string>
#include <string_view>
#include "SQLiteCpp/Database.h"
namespace db {

class database_item {
public:
  friend class database;
  virtual ~database_item() = default;

  // Constructor
  // Used for subclass
  database_item() = default;
  database_item(std::string_view id) : _id{id} {}

  // Getters
  std::string get_id() const { return _id; }

  // Helpful Function
  // Items without IDs are not saved in DB
  bool saved_in_db() const { return _id != ""; }

protected:
  std::string _id;

private:
  virtual bool add_to_database(SQLite::Database& db) = 0;
  virtual bool remove_from_database(SQLite::Database& db) = 0;
  virtual bool update_in_database(SQLite::Database& db, std::map<std::string, std::any> props) = 0;
};
} // namespace db
