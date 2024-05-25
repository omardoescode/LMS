#include "auth/instructor.h"
#include "db/database.h"
#include "db/database_item.h"
#include "utils/split_string.h"

#include <algorithm>
#include <any>
#include <iostream>
#include <learn/course.h>
#include <learn/course_registration.h>
#include <memory>
#include <utils/exceptions.h>

namespace learn {

course::course (std::string id) : db::database_item (id) {
    get ();
}
course::course (std::string name,
std::string professor,
std::string textbook,
int credit_hours,
std::string course_code)
: _name{ name }, _professor (professor), _textbook (textbook),
  _credit_hours (credit_hours), _course_code (course_code) {
}
auth::instructor course::get_professor () {
    return auth::instructor (_professor);
}
utils::vector<std::unique_ptr<auth::instructor>> course::get_teaching_assistants () {
    utils::vector<std::unique_ptr<auth::instructor>> res;
    for (const auto& ta : _teaching_assistants)
        res.push_back (std::make_unique<auth::instructor> (ta));
    return res;
}
utils::vector<std::unique_ptr<auth::student>> course::get_students () {
    utils::vector<std::unique_ptr<auth::student>> res;
    for (const auto& student : _students)
        res.push_back (std::make_unique<auth::student> (student));
    return res;
}
bool course::set_credit_hours (int new_value) {
    if (new_value <= 0)
        throw utils::custom_exception{
            "Credit hours cannot be less than or equal to 0"
        };

    _credit_hours = new_value;
    return db::database::get_instance ().update_item (
    *this, { { "credit_hours"s, std::to_string (new_value) } });
}
utils::vector<std::unique_ptr<learn::assignment>> course::get_assignments () {
    utils::vector<std::unique_ptr<learn::assignment>> res;
    for (const auto& assignment : _assignments)
        res.push_back (std::make_unique<learn::assignment> (assignment));
    return res;
}

utils::vector<std::unique_ptr<course>> course::getCourses (
std::map<std::string, std::string> props) {
    std::string query_string =
    "select courses.* from courses left join instructors_courses on courses.id "
    "= instructors_courses.course_id left join instructors on "
    "instructors_courses.instructor_id = instructors.id left join "
    "students_courses on courses.id = students_courses.course_id left join "
    "students on students_courses.student_id = students.id";

    utils::vector<std::unique_ptr<course>> results;

    SQLite::Statement query (db::database::get_db (), query_string);

    if (!props.empty ()) {
        int ind = props.size () - 1;
        query_string += " where ";
        for (auto const& p : props) {
            query_string += p.first + " = ?" + (ind > 0 ? " and " : "");
            ind--;
        }

        query_string += " group by courses.id";

        query = SQLite::Statement (db::database::get_db (), query_string);
        int i = 1;

        for (auto const& p : props) {
            query.bind (i++, p.second);
        }
    } else {
        query_string += " group by courses.id";
        query = SQLite::Statement (db::database::get_db (), query_string);
    }

    while (query.executeStep ()) {
        std::string course_id = (std::string)query.getColumn (0);
        std::unique_ptr<course> s (new course (course_id));
        results.push_back (std::move (s));
    }

    return results;
}

void course::get () {
    if (!saved_in_db ())
        throw utils::custom_exception{ "Item not saved in database;" };

    std::string query_string =
    "select courses.*, (select instructors.id from instructors join "
    "instructors_courses on instructors.id = instructors_courses.instructor_id "
    "where instructors_courses.course_id = courses.id and "
    "instructors.is_teaching_assistant = 0 limit 1) as instructor, "
    "group_concat(distinct case when instructors.is_teaching_assistant = 1 "
    "then instructors.id end) as teaching_assistants, group_concat(distinct "
    "students.id) as students, group_concat(distinct assignments.id) as "
    "assignments from courses left join instructors_courses on courses.id = "
    "instructors_courses.course_id left join instructors on "
    "instructors_courses.instructor_id = instructors.id left join "
    "students_courses on courses.id = students_courses.course_id left join "
    "students on students_courses.student_id = students.id left join "
    "assignments on assignments.course_id = courses.id where courses.id = ? "
    "group by courses.id ";


    SQLite::Statement query (db::database::get_db (), query_string);
    query.bind (1, _id);

    while (query.executeStep ()) {

        _id           = (std::string)query.getColumn (0);
        _name         = (std::string)query.getColumn (1);
        _credit_hours = query.getColumn (2);
        _textbook     = (std::string)query.getColumn (3);
        _course_code  = (std::string)query.getColumn (4);
        _professor    = (std::string)query.getColumn (5);

        std::string teaching_assistants_ids = query.getColumn (6);
        _teaching_assistants = utils::split_string (teaching_assistants_ids, ',');

        std::string students_ids = query.getColumn (7);
        _students                = utils::split_string (students_ids, ',');

        std::string assignments_ids = query.getColumn (8);
        _assignments = utils::split_string (assignments_ids, ',');

#if PRINT_DATA_WHEN_RETRIEVED
        std::cout << "Course: " << _name << "\nID: " << _id
                  << "\nCredit Hours: " << _credit_hours << "\nText Book: " << _textbook
                  << "\nInstructor: " << _professor << "\nCourse Code: " << _course_code
                  << "\nTeaching Assistants: " << teaching_assistants_ids
                  << "\nStudents: " << students_ids
                  << "\nAssignments: " << assignments_ids << std::endl
                  << std::endl;
#endif
    }
}

bool course::add_to_database (SQLite::Database& db) {
    SQLite::Statement query (
    db, "INSERT INTO Courses(name, credit_hours, text_book, course_code) VALUES(?,?, ?, ?) RETURNING id");

    query.bindNoCopy (1, _name);
    query.bind (2, _credit_hours);
    query.bindNoCopy (3, _textbook);
    query.bindNoCopy (4, _course_code);

    query.executeStep ();
    int course_id = query.getColumn (0);

    int success = auth::instructor (_professor).add_course (std::to_string (course_id));

    _id = course_id;
    return (course_id != 0) && (success);
}

bool course::remove_from_database (SQLite::Database& db) {
    SQLite::Statement query (db, "DELETE FROM Courses WHERE id = ?");

    query.bind (1, _id);

    int success = query.exec ();

    return success;
}
bool course::update_in_database (SQLite::Database& db, std::map<std::string, std::any> props) {
    std::string query_string = "UPDATE Courses SET ";
    int count                = 0;
    for (auto const& [key, val] : props) {
        if (count > 0)
            query_string += ",";


        if (key == "credit_hours") {
            query_string += key + " = " + std::to_string (std::any_cast<int> (val));
        } else {
            query_string += key + " = '" + std::any_cast<std::string> (val) + "'";
        }

        count++;
    }

    query_string += " WHERE id = ?";

    SQLite::Statement query (db, query_string);
    query.bindNoCopy (1, _id);

    int success = query.exec ();
    return success;
}


bool course::has_teaching_assistant (const auth::instructor& TA) const {
    for (const auto& ta : _teaching_assistants) {
        std::cout << ta << " == " << TA.get_id () << std::endl;
        if (ta == TA.get_id ()) {
            std::cout << ta << " === " << TA.get_id () << std::endl;
            return true;
        }
    }
    return false;
}


utils::vector<std::unique_ptr<learn::course_registration>> course::get_registrations () {
    utils::vector<std::unique_ptr<learn::course_registration>> res;
    for (const std::string& registration : _registrations)
        res.push_back (std::make_unique<course_registration> (registration));
    return res;
}


} // namespace learn
