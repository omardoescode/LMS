#include "learn/course_registration.h"
#include "auth/student.h"
#include "db/database.h"
#include "learn/course.h"

#include <iostream>
namespace learn {
course_registration::course_registration (std::string id)
: db::database_item (id) {
    get ();
}
course_registration::course_registration (std::string course, std::string student, CourseRegistrationState state)
: _course{ course }, _student{ student }, _state{ state } {
}
bool course_registration::course_registration::set_state (CourseRegistrationState new_state) {
    _state = new_state;
    if (db::database::get_instance ().update_item (*this, { { "state", new_state } })) {
        _state = new_state;
        return true;
    }
    return false;
}

course_registration::CourseRegistrationState
course_registration::enum_translate (std::string s) {
    if (s == "Enrolled" || s == "Current")
        return CourseRegistrationState::ENROLLED;
    if (s == "Dropped")
        return CourseRegistrationState::DROPPED;
    if (s == "Finished")
        return CourseRegistrationState::FINISHED;
    if (s == "AwaitingApproval")
        return CourseRegistrationState::AWAITING_APPROVAL;
}

std::string course_registration::enum_translate (CourseRegistrationState r) {
    if (r == CourseRegistrationState::ENROLLED)
        return "Enrolled";
    if (r == CourseRegistrationState::DROPPED)
        return "Dropped";
    if (r == CourseRegistrationState::FINISHED)
        return "Finished";
    if (r == CourseRegistrationState::AWAITING_APPROVAL)
        return "AwaitingApproval";
}

utils::vector<std::unique_ptr<course_registration>>
course_registration::getCourseRegistrations (std::map<std::string, std::string> props) {
    std::string query_string =
    "select students_courses.id from students_courses join students on "
    "students.id = students_courses.student_id  join users on users.id = "
    "students.user_id join courses on courses.id = students_courses.course_id";

    utils::vector<std::unique_ptr<course_registration>> results;

    SQLite::Statement query (db::database::get_db (), query_string);

    if (!props.empty ()) {
        int ind = props.size () - 1;
        query_string += " where ";
        for (auto const& p : props) {
            query_string += p.first + " = ?" + (ind > 0 ? " and " : "");
            ind--;
        }

        query = SQLite::Statement (db::database::get_db (), query_string);
        int i = 1;

        for (auto const& p : props) {
            query.bind (i++, p.second);
        }
    }

    while (query.executeStep ()) {
        std::string course_registration_id = (std::string)query.getColumn (0);
        std::unique_ptr<course_registration> s (
        new course_registration (course_registration_id));
        results.push_back (std::move (s));
    }

    return results;
}


void course_registration::get () {
    if (!saved_in_db ())
        throw utils::custom_exception{ "Item not saved in database;" };

    std::string query_string = "select id, student_id, course_id, state from "
                               "students_courses where id = ?";

    SQLite::Statement query (db::database::get_db (), query_string);
    query.bind (1, _id);

    while (query.executeStep ()) {
        _student = (std::string)query.getColumn (1);
        _course  = (std::string)query.getColumn (2);
        _state   = enum_translate (query.getColumn (3));

#if PRINT_DATA_WHEN_RETRIEVED
        std::cout << "Student ID: " << _student << "\nCourse ID: " << _course
                  << "\nState: " << enum_translate (_state) << std::endl
                  << std::endl;
#endif
    }
}
bool course_registration::add_to_database (SQLite::Database& db) {
    SQLite::Statement query (
    db, "INSERT INTO Students_Courses(student_id, course_id, state) VALUES(?,?, ?) RETURNING id");

    query.bindNoCopy (1, _student);
    query.bindNoCopy (2, _course);
    query.bindNoCopy (3, enum_translate (_state));

    query.executeStep ();
    int course_reg_id = query.getColumn (0);

    _id = course_reg_id;
    return (course_reg_id != 0);
}
bool course_registration::remove_from_database (SQLite::Database& db) {
    SQLite::Statement query (db, "DELETE FROM Students_Courses WHERE id = ?");

    query.bindNoCopy (1, _id);

    int success = query.exec ();
    return success;
}
bool course_registration::update_in_database (SQLite::Database& db,
std::map<std::string, std::any> props) {
    std::string query_string = "UPDATE Students_Courses SET ";
    int count                = 0;
    for (auto const& [key, val] : props) {
        if (count > 0)
            query_string += ",";
        query_string += key + " = '" + std::any_cast<std::string> (val) + "'";
        count++;
    }

    query_string += " WHERE id = ?";

    SQLite::Statement query (db, query_string);
    query.bindNoCopy (1, _id);

    int success = query.exec ();
    return success;
}

learn::course course_registration::get_course () {
    return learn::course (_course);
};

auth::student course_registration::get_student () {
    return auth::student (_student);
};
} // namespace learn
