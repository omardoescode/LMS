#include "db/database.h"
#include "db/database_item.h"
#include <string_view>
namespace db {
database::database() {
  // Set connection key or whatever way possible
  // TODO: read connection key from a file
  initialize_db("");
};
void database::initialize_db(std::string_view connection_key) {
  _connection_key = connection_key;
  // TODO
}
} // namespace db
