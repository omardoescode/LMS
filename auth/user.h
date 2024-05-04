#pragma once
#include "db/db.h"
namespace auth {
class user : public db::database_item {
public:
  // Consturctor
  user(std::string username, std::string email,
       std::string password);     //  Will include adding it to the database
  explicit user(std::string _id); // Load from the database

  // Getters
  std::string get_username() const { return _username; }
  std::string get_email() const { return _email; }

  // Setters
  // When applied, we need to update in DB as well
  bool set_username(std::string_view);
  bool set_email(std::string_view);

  // Helpful Functions
  bool check_password(std::string password);
  bool saved_in_db() const { return _id != ""; }

protected:
  std::string _id, _name, _username, _password_hash, _email, _faculty;
};
} // namespace auth
