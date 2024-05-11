#include "auth/instructor.h"
#include "db/database.h"
#include "db/database_item.h"
#include "utils/split_string.h"

#include <algorithm>
#include <any>
#include <iostream>
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
/*
utils::vector<std::unique_ptr<course>> course::get (
std::map<std::string, std::any> filtering_props) {
    utils::vector<std::unique_ptr<course>> courses_objs;

    std::string query_string =
    "select distinct courses.*, (select instructors.id from instructors join "
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
    "instructors_courses";

    std::string formated_query_string =
    filtering_props.size () ? query_string + " where" : query_string;

    if (filtering_props.find ("id") != filtering_props.end ())
        formated_query_string += " courses.id = '" +
        std::any_cast<std::string> (filtering_props["id"]) + "' and";
    if (filtering_props.find ("credit_hourse") != filtering_props.end ())
        formated_query_string += " courses.credit_hourse = '" +
        std::any_cast<std::string> (filtering_props["credit_hourse"]) + "' and";
    if (filtering_props.find ("name") != filtering_props.end ())
        formated_query_string += " courses.name like '%" +
        std::any_cast<std::string> (filtering_props["name"]) + "%' and";
    if (filtering_props.find ("text_book") != filtering_props.end ())
        formated_query_string += " courses.text_book = '" +
        std::any_cast<std::string> (filtering_props["text_book"]) + "' and";
    if (filtering_props.find ("professor") != filtering_props.end ())
        formated_query_string += " instructor like '%" +
        std::any_cast<std::string> (filtering_props["professor"]) + "%' and";
    if (filtering_props.find ("teaching_assistant") != filtering_props.end ())
        formated_query_string += " teaching_assistants like '%" +
        std::any_cast<std::string> (filtering_props["text_book"]) + "%' and";
    if (filtering_props.find ("student") != filtering_props.end ())
        formated_query_string += " students like '%" +
        std::any_cast<std::string> (filtering_props["student"]) + "%' and";


    formated_query_string += filtering_props.size () ? " 1=1" : " order by courses.id";

    SQLite::Statement query (db::database::get_db (), formated_query_string);

    while (query.executeStep ()) {

        int course_id                       = query.getColumn (0);
        std::string name                    = query.getColumn (1);
        int credit_hours                    = query.getColumn (2);
        std::string text_book               = query.getColumn (3);
        std::string instructor_id           = query.getColumn (4);
        std::string teaching_assistants_ids = query.getColumn (5);
        utils::vector<std::string> teaching_assistants =
        utils::split_string (teaching_assistants_ids, ',');
        std::string students_ids = query.getColumn (6);
        utils::vector<std::string> students = utils::split_string (students_ids, ',');

        std::unique_ptr<course> course_data_ptr =
        std::make_unique<course> (std::to_string (course_id), name,
        instructor_id, text_book, credit_hours, teaching_assistants, students);

        courses_objs.push_back (std::move (course_data_ptr));

        std::cout << "Course: " << name << "\nID: " << course_id
                  << "\nCredit Hourse: " << credit_hours
                  << "\nText Book: " << text_book << "\nInstructor: " << instructor_id
                  << "\nTeaching Assistants: " << teaching_assistants_ids
                  << "\nStudents: " << students_ids << std::endl
                  << std::endl;
    }

    return courses_objs;
*/
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
