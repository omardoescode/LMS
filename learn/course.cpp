#include "db/database.h"
#include "db/database_item.h"
#include <any>
#include <learn/course.h>
#include <utils/exceptions.h>

namespace learn {
course::course (std::string id,
std::string name,
std::string professor,
std::string textbook,
int credit_hours,
utils::vector<std::string> teaching_assistants,
utils::vector<std::string> students)
: db::database_item (id), _name{ name }, _professor (professor),
  _textbook (textbook), _credit_hours (credit_hours) {
    _teaching_assistants = std::move (teaching_assistants);
    _students            = std::move (students);
}
// auth::instructor course::get_professor () {
//     // return db::database::get_instance ().get_elem_by_id<auth::instructor> (
//     // "instructors", _professor);
//     auto instructors = auth::instructor::get ({ { "id", _professor } });
//     if (instructors.empty ())
//         throw utils::custom_exception{
//             "Invalid professor id, this professor doesn't exist in DB"
//         };
//     return *instructors[0];
// }
//
// std::list<auth::student> course::get_teaching_assistants () const {
//     return db::database::get_instance ().get_elems_by_id<auth::instructor> (
//     "instructors", _teaching_assistants);
// }
// std::list<auth::student> course::get_students () const {
//     return db::database::get_instance ().get_elems_by_id<auth::student> (
//     "students", _students);
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
//
// std::list<learn::assignment> course::get_assigments () const {
//     std::map<std::string, std::string> props;
//     props["course"] = _id;
//     auto res        = db::database::get_instance ().get ("assignments", props);
//     std::list<learn::assignment> res_casted;
//     for (auto& elem : res)
//         res_casted.push_back (dynamic_cast<learn::assignment&> (elem));
//
//     return res_casted;
// }


utils::vector<std::unique_ptr<course>> course::get (
std::map<std::string, std::any> filtering_props) {
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
