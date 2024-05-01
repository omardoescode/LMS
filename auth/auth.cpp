#include "auth.h"
#include "../db/db.h"

namespace auth {
bool user::set_username(std::string_view username) {
  std::map<std::string, std::string> new_props;
  new_props["username"] = username;
  _username = username;
  return db::database::get_instance().update_item(*this, new_props);
}

bool student::update_in_database(std::map<std::string, std::string>) {
  return true;
}
} // namespace auth
