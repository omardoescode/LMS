#include "auth/instructor.h"
#include "db/database.h"
#include "db/database_item.h"
#include <algorithm>
#include <any>
#include <learn/course.h>
#include <memory>
#include <utils/exceptions.h>

namespace learn {

course::course (std::string id) : db::database_item (id) {
    get ();
}
course::course (std::string name, std::string professor, std::string textbook, int credit_hours)
: _name{ name }, _professor (professor), _textbook (textbook),
  _credit_hours (credit_hours) {
}
auth::instructor course::get_professor () {
    return auth::instructor (_professor);
}
// utils::vector<std::unique_ptr<auth::instructor>> course::get_teaching_assistants () {
//     utils::vector<std::unique_ptr<auth::instructor>> res;
//     for (const auto& ta : _teaching_assistants)
//         res.push_back (std::make_unique<auth::instructor> (ta));
//     return res;
// }
// utils::vector<std::unique_ptr<auth::student>> course::get_students () {
//     utils::vector<std::unique_ptr<auth::student>> res;
//     for (const auto& student : _students)
//         res.push_back (std::make_unique<auth::student> (student));
//     return res;
// }
//
// bool course::set_credit_hours (int new_value) {
//     if (new_value <= 0)
//         throw utils::custom_exception{
//             "Credit hours cannot be less than or equal to 0"
//         };
//
//     _credit_hours = new_value;
//     std::map<std::string, std::any> props;
//     props["credit_hours"] = new_value;
//     return db::database::get_instance ().update_item (*this, props);
// }
// utils::vector<std::unique_ptr<learn::assignment>> course::get_assigments () {
//     utils::vector<std::unique_ptr<learn::assignment>> res;
//     for (const auto& assignment : _assignments)
//         res.push_back (std::make_unique<learn::assignment> (assignment));
//     return res;
// }

void course::get () {
}
bool course::add_to_database (SQLite::Database& db) {
    return true;
}

bool course::remove_from_database (SQLite::Database& db) {
    return true;
}
bool course::update_in_database (SQLite::Database& db, std::map<std::string, std::any> props) {
    return true;
}
} // namespace learn
