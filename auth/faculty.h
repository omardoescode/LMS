#pragma once
#include "auth/administrator.h"
#include "auth/student.h"
#include "db/database_item.h"
#include "utils/vector.h"
#include <map>
#include <memory>
namespace auth {
class faculty : public db::database_item {
public:
    // Constructors
    explicit faculty (std::string id);
    faculty (std::string name, std::string administrator);

    // Getters
    std::string get_name () const {
        return _name;
    }

    // Search in DB and return the values where faculty is this instance
    auth::administrator get_administrator ();
    utils::vector<std::unique_ptr<auth::instructor>> get_instructors ();
    utils::vector<std::unique_ptr<auth::student>> get_students ();
    utils::vector<std::unique_ptr<learn::course>> get_courses ();

    // vpverridden Functions
    bool add_to_database (SQLite::Database& db) override;
    bool remove_from_database (SQLite::Database& db) override;
    bool update_in_database (SQLite::Database& db,
    std::map<std::string, std::any> props) override;
    void get () override;

private:
    std::string _name, _administrator;

    utils::vector<std::string> _students, _instructors, _courses;
};

} // namespace auth
