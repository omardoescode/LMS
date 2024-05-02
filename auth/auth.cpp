#include "auth.h"
#include "../db/db.h"
#include <any>

namespace auth {
bool user::set_username(std::string_view username) {
  std::map<std::string, std::any> new_props;
  new_props["username"] = username;
  _username = username;
  return db::database::get_instance().update_item(*this, new_props);
}

} // namespace auth
