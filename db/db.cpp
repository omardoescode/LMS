#include "db.h"
namespace db {
database::database() {
  // Set connection key or whatever way possible
  // TODO: read connection key from a file
  initialize_db("");
};
void database::initialize_db(std::string connection_key) {
  _connection_key = connection_key;
  // TODO
}
} // namespace db