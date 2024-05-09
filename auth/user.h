#pragma once
#include "SQLiteCpp/Database.h"
#include "db/database_item.h"

namespace auth {
class user : public db::database_item {
public:
  // Consturctor
  // Will be used for construction of the subclasses
  // Simply assigning parameters to properties of the object using member
  // initializer list
  user(std::string username, std::string email, std::string password);

  // Intialize as well
  // Used for subclasses
  explicit user(std::string_view id) : database_item(id){};

  // Getters
  std::string get_username() const { return _username; }
  std::string get_email() const { return _email; }

  // Setters
  // When applied, we need to update in DB as well
  bool set_username(std::string_view new_username);
  bool set_email(std::string_view new_email);

  // Helpful Functions
  bool check_password(std::string password);

  // Overriden Functions
  virtual bool add_to_database(SQLite::Database &db) = 0;
  virtual bool remove_from_database(SQLite::Database &db) = 0;
  virtual bool update_in_database(SQLite::Database &db,
                                  std::map<std::string, std::any> props) = 0;

protected:
  std::string _name, _username, _password_hash, _email, _faculty;
};
} // namespace auth
