#pragma once
#include <any>
#include <map>
#include <string>
namespace db {

class database_item {
private:
  virtual bool add_to_database() = 0;
  virtual bool remove_from_database() = 0;
  virtual bool update_in_database(std::map<std::string, std::any> props) = 0;

public:
  friend class database;
  virtual ~database_item() = default;
};
} // namespace db
