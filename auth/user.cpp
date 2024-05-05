#include "auth/user.h"

namespace auth {
  const std::hash<std::string> hasher;
  user::user(std::string username, std::string email, std::string password): _username{username}, _email{email},_password_hash{std::to_string(hasher(password))} {
  }
}