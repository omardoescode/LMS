#include "db/database.h"
#include "db/database_item.h"
#include <any>
#include <iostream>
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
    utils::vector<std::unique_ptr<course>> courses_objs;

    std::string query_string =
    "select courses.*, (select instructors.id from instructors join "
    "instructors_courses on instructors.id == "
    "instructors_courses.instructor_id where instructors_courses.course_id == "
    "courses.id and instructors.is_teaching_assistant == 0 ) as instructor "
    ",((select group_concat(instructors.id) from instructors join "
    "instructors_courses on instructors.id == "
    "instructors_courses.instructor_id where instructors_courses.course_id == "
    "courses.id and instructors.is_teaching_assistant == 1 )) as "
    "teaching_assistants, ((select group_concat(students.id) from students "
    "join students_courses on students.id == students_courses.student_id where "
    "students_courses.course_id == courses.id)) as students from courses join "
    "instructors_courses on instructors_courses.course_id == courses.id";

    std::string formated_query_string =
    filtering_props.size () ? query_string + " where" : query_string;

    if (filtering_props.find ("id") != filtering_props.end ())
        formated_query_string += " courses.id = '" +
        std::any_cast<std::string> (filtering_props["id"]) + "' and";
    if (filtering_props.find ("credit_hourse") != filtering_props.end ())
        formated_query_string += " courses.credit_hourse = '" +
        std::any_cast<std::string> (filtering_props["credit_hourse"]) + "' and";
    if (filtering_props.find ("name") != filtering_props.end ())
        formated_query_string += " courses.name = '" +
        std::any_cast<std::string> (filtering_props["name"]) + "' and";
    if (filtering_props.find ("text_book") != filtering_props.end ())
        formated_query_string += " courses.text_book = '" +
        std::any_cast<std::string> (filtering_props["text_book"]) + "' and";

    formated_query_string += filtering_props.size () ? " 1=1" : "";

    SQLite::Statement query (db::database::get_db (), formated_query_string);

    while (query.executeStep ()) {

        int course_id                       = query.getColumn (0);
        std::string name                    = query.getColumn (1);
        int credit_hours                    = query.getColumn (2);
        std::string text_book               = query.getColumn (3);
        std::string instructor_id           = query.getColumn (4);
        std::string teaching_assistants_ids = query.getColumn (5);
        std::string students_ids            = query.getColumn (6);

        std::unique_ptr<course> course_data_ptr =
        std::make_unique<course> (course_id, name, credit_hours, text_book);

        courses_objs.push_back (std::move (course_data_ptr));

        std::cout << "Course: " << name << "\nID: " << course_id
                  << "\nCredit Hourse: " << credit_hours
                  << "\nText Book: " << text_book << std::endl
                  << "\nInstructor: " << instructor_id << std::endl
                  << "\nTeaching Assistants: " << teaching_assistants_ids << std::endl
                  << "\nStudents: " << students_ids << std::endl
                  << std::endl;
    }

    return courses_objs;
}


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
