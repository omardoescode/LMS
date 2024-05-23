#include "auth/instructor.h"

#include "learn/assignment.h"
#include "learn/assignment_submission.h"
#include "learn/course_registration.h"
#include "utils/exceptions.h"
#include "utils/split_string.h"

#include <iostream>

namespace auth {

instructor::instructor (std::string id) : user (id) {
    get ();
}

instructor::instructor (std::string name,
std::string faculty,
std::string email,
std::string password,
bool is_teaching_assistant)
: user (name, faculty, email, password, Role::INSTRUCTOR),
  _is_teaching_assistant (is_teaching_assistant) {
}

bool instructor::add_course (std::string_view course_id) {
    // Check if the course already exists in the instructor's list of courses
    for (const std::string& existing_course : _courses) {
        if (existing_course == std::string (course_id)) {
            // Course already exists, so return false indicating failure to add
            return false;
        }
    }

    // If the course is not already in the list, add it to the instructor's courses
    _courses.push_back (std::string (course_id));

    // Update the database with the new course assignment for the instructor
    // Still not sure how we're handling the DB but: EX. db::database::get_instance().add_course_for_instructor(_id, course_id);

    return true; // Course successfully added
}

bool instructor::remove_course (std::string_view course_id) {
    bool courseRemoved = false;

    // Find the position of the course in the instructor's list of courses
    auto it = std::find (_courses.begin (), _courses.end (), std::string (course_id));

    if (it != _courses.end ()) {
        // Course found, erase it from the list
        _courses.erase ((it - _courses.begin ()));

        // Update the database to remove the course assignment for the instructor
        // Still not sure how we're handling the DB but: EX. db::database::get_instance().remove_course_for_instructor(_id, course_id);

        courseRemoved = true;
    }

    return courseRemoved;
}


bool instructor::add_student (auth::student& student, learn::course course) {
    // Check if the student is already registered for the course
    for (const auto& registration : course.get_registrations ()) {
        if (registration->get_student ().get_id () == student.get_id ()) {
            // Student is already registered, do nothing
            return false;
        }
    }

    // Create a new course registration with the state Enrolled
    learn::course_registration newRegistration (course.get_id (), student.get_id (),
    learn::course_registration::CourseRegistrationState::ENROLLED);


    // Add the registration to the db
    db::database::get_instance ().add_item (newRegistration);

    return true; // Student successfully added to the course
}

bool instructor::add_teaching_assistant (std::string course_id, auth::instructor& TA) {
    // Find the course with the given course_id
    auto course = learn::course (course_id);

    // Check if the TA is already assigned as a teaching assistant for the course
    if (course.has_teaching_assistant (TA)) {
        // TA is already a teaching assistant for the course, do nothing
        return false;
    }

    // Assign the TA as a teaching assistant for the course
    // TODO: Handle adding the TA to the course
    // add_teaching_assistant (_id, TA);

    // Update the database with the new teaching assistant assignment for the course
    SQLite::Statement query (db::database::get_db (),
    "insert into Instructors_Courses (instructor_id, course_id) values(? , ?)");
    query.bind (1, TA.get_id ());
    query.bind (2, course_id);

    int success = query.exec ();

    return success; // Teaching assistant successfully added to the course
}

// int instructor::get_maximum_grade (std::string course_id) {
//     // Find the course with the given course_id
//     learn::course* course = db::database::get_instance ().get_course_by_id (course_id);
//
//     if (course) {
//         // Get the maximum grade for the course
//         int maxGrade = course->get_maximum_grade ();
//
//         return maxGrade;
//     }
//
//     // Return a default value or handle error as needed
//     return -1; // Default value indicating course not found or error
// }
//
//
// int instructor::get_minimum_grade (std::string course_id) {
//     // Find the course with the given course_id
//     learn::course* course = db::database::get_instance ().get_course_by_id (course_id);
//
//     if (course) {
//         // Get the minimum grade for the course
//         int minGrade = course->get_minimum_grade ();
//
//         return minGrade;
//     }
//
//     // Return a default value or handle error as needed
//     return -1; // Default value indicating course not found or error
// }
//
//
// double instructor::get_average_grade (std::string course_id) {
//     // Find the course with the given course_id
//     learn::course* course = db::database::get_instance ().get_course_by_id (course_id);
//
//     if (course) {
//         // Get the average grade for the course
//         double averageGrade = course->get_average_grade ();
//
//         return averageGrade;
//     }
//
//     // Return a default value or handle error as needed
//     return -1.0; // Default value indicating course not found or error
// }


bool modify_grade (learn::assignment_submission& submission, double grade) {
    return db::database::get_instance ().update_item (
    submission, { { "grade", std::to_string (grade) } });
}

bool instructor::modify_grade (auth::student& student, learn::assignment& assignment, double grade) {
    try {
        // TODO DB: Find the submission that has this student and this
        // assignment Do this by creating a static method in
        // learn::assignment_submission If not found throw a
        // utils::custom_exception then complete this part
        // By just calling the other version of this function by giving that submission you got and the new grade
    } catch (utils::custom_exception&) {
        auto submission =
        learn::assignment_submission (assignment.get_id (), student.get_id (), grade);
        db::database::get_instance ().add_item (submission);
    }
    return true;
}

utils::vector<std::unique_ptr<instructor>> instructor::getInstructors (
std::map<std::string, std::string> props) {
    std::string query_string = "select instructors.id from instructors join "
                               "users on users.id == instructors.user_id";

    utils::vector<std::unique_ptr<instructor>> results;

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
        std::string instructor_id = (std::string)query.getColumn (0);
        std::unique_ptr<instructor> s (new instructor (instructor_id));
        results.push_back (std::move (s));
    }

    return results;
}

void instructor::get () {
    if (!saved_in_db ())
        throw utils::custom_exception{ "Item not saved in database;" };

    std::string query_string =
    "select users.*, instructors.id as instructor_id, "
    "instructors.is_teaching_assistant, (select "
    "group_concat(instructors_courses.course_id) from instructors_courses "
    "where instructors_courses.instructor_id = instructors.id) as courses "
    "from "
    "instructors join users on users.id == instructors.user_id where "
    "instructors.id = ?";

    SQLite::Statement query (db::database::get_db (), query_string);
    query.bind (1, _id);

    while (query.executeStep ()) {

        _password_hash          = (std::string)query.getColumn (1);
        _email                  = (std::string)query.getColumn (2);
        _faculty                = (std::string)query.getColumn (3);
        _name                   = (std::string)query.getColumn (4);
        std::string role_string = (std::string)query.getColumn (5);
        _role                   = string_to_role (role_string);
        _is_teaching_assistant  = query.getColumn (7).getInt ();
        std::string courses_ids = query.getColumn (8);
        _courses                = utils::split_string (courses_ids, ',');

#if PRINT_DATA_WHEN_RETRIEVED
        std::cout
        << "Instructor: " << _name << "\nID: " << _id << "\nEmail: " << _email
        << "\nFaculty: " << _faculty
        << "\nTeaching Assistant: " << (_is_teaching_assistant ? "Yes" : "No")
        << "\nRole: " << role_string << "\nCourses: " << courses_ids << std::endl
        << std::endl;
#endif
    }
}
bool instructor::add_to_database (SQLite::Database& db) {
    SQLite::Statement query (
    db, "INSERT INTO Users(name, password_hash, email, faculty, role) VALUES(?,?, ?, ?, ?) RETURNING id");

    query.bindNoCopy (1, _name);
    query.bindNoCopy (2, _password_hash);
    query.bindNoCopy (3, _email);
    query.bindNoCopy (4, _faculty);
    query.bindNoCopy (5, role_to_string (_role));

    query.executeStep ();
    int user_id = query.getColumn (0);

    std::string id = _name;
    std::replace (id.begin (), id.end (), ' ', '.');

    query = SQLite::Statement (
    db, "INSERT INTO Instructors(id, user_id, is_teaching_assistant) VALUES(?,?,?)");
    query.bind (1, id);
    query.bind (2, user_id);
    query.bind (3, _is_teaching_assistant);

    int success = query.exec ();
    _id         = id;
    return success;
}

// TODO DB: Integrate database handling with existing add_course and remove_course

// DATABASE HANDLING FOR add_course and remove_course
/*
bool instructor::add_course (std::string course_id) {
    SQLite::Statement query (db::database::get_instance ().get_db (),
    "INSERT INTO Instructors_Courses(instructor_id, course_id) VALUES(?,?) "
    "RETURNING id");

    query.bindNoCopy (1, _id);
    query.bindNoCopy (2, course_id);

    query.executeStep ();
    int course_reg_id = query.getColumn (0);

    _id = course_reg_id;
    return (course_reg_id != 0);
}

bool instructor::remove_course (std::string course_id) {
    SQLite::Statement query (db::database::get_instance ().get_db (),
    "DELETE FROM Instructors_Courses WHERE instructor_id = ?, course_id = ?");

    query.bindNoCopy (1, _id);
    query.bindNoCopy (2, course_id);

    int success = query.exec ();
    return success;
}
*/

bool instructor::remove_from_database (SQLite::Database& db) {
    SQLite::Statement query (db,
    "DELETE FROM Users WHERE Users.id = (SELECT Users.id FROM Users JOIN "
    "Instructors ON Instructors.user_id == Users.id WHERE Instructors.id = ?)");
    query.bindNoCopy (1, _id);

    int success = query.exec ();
    return success;
}
bool instructor::update_in_database (SQLite::Database& db,
std::map<std::string, std::any> props) {
    std::string query_string = "UPDATE Users SET ";
    int count                = 0;
    for (auto const& [key, val] : props) {
        if (count > 0)
            query_string += ",";
        query_string += key + " = '" + std::any_cast<std::string> (val) + "'";
        count++;
    }

    query_string +=
    " WHERE id = (SELECT Users.id FROM Users JOIN "
    "Insturctors ON Insturctors.user_id == Users.id WHERE Insturctors.id = ?)";

    SQLite::Statement query (db, query_string);
    query.bindNoCopy (1, _id);

    int success = query.exec ();
    return success;
}
} // namespace auth
