#include "db/database.h"
#include <any>
#include <learn/course.h>
#include <utils/exceptions.h>

namespace learn {
// auth::instructor course::get_professor() const {
//   return db::database::get_instance().get_elem_by_id<auth::instructor>(
//       "instructors", _professor);
// }

// std::list<auth::student> course::get_teaching_assistants() const {
//   return db::database::get_instance().get_elems_by_id<auth::instructor>(
//       "instructors", _teaching_assistants);
// }
// std::list<auth::student> course::get_students() const {
//   return
//   db::database::get_instance().get_elems_by_id<auth::student>("students",
//                                                                      _students);
// }

// bool course::set_credit_hours(int new_value) {
//   if (new_value <= 0)
//     throw utils::custom_exception{
//         "Credit hours cannot be less than or equal to 0"};
//
//   _credit_hours = new_value;
//   std::map<std::string, std::any> props;
//   props["credit_hours"] = new_value;
//   return db::database::get_instance().update_item(*this, props);
// }
//
// std::list<learn::assignment> course::get_assigments() const {
//   std::map<std::string, std::string> props;
//   props["course"] = _id;
//   auto res = db::database::get_instance().get("assignments", props);
//   std::list<learn::assignment> res_casted;
//   for (auto &elem : res)
//     res_casted.push_back(dynamic_cast<learn::assignment &>(elem));
//
//   return res_casted;
// }

} // namespace learn
