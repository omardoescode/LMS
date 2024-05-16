#include "auth/instructor.h"

#include "utils/split_string.h"

#include <iostream>

namespace auth {

instructor::instructor (std::string id) : user (id) {
    get ();
}

instructor::instructor (std::string username, std::string email, std::string password_hash)
: user (username, email, password_hash) {
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
        _courses.erase (*it);

        // Update the database to remove the course assignment for the instructor
        // Still not sure how we're handling the DB but: EX. db::database::get_instance().remove_course_for_instructor(_id, course_id);

        courseRemoved = true;
    }

    return courseRemoved;
}


bool instructor::add_student (auth::student& student, learn::course course) {
    // Check if the student is already registered for the course
    for (const auto& registration : course.get_registrations ()) {
        if (registration.get_student_id () == student.get_id ()) {
            // Student is already registered, do nothing
            return false;
        }
    }

    // Create a new course registration with the state Enrolled
    learn::course_registration newRegistration (
    course.get_id (), student.get_id (), learn::registration_state::Enrolled);

    // Add the student to the course registration
    course.add_registration (newRegistration);

    // Save the new course registration in the database (subject to db command change?)
    db::database::get_instance ().add_registration_to_course (newRegistration);

    return true; // Student successfully added to the course
}

bool instructor::add_teaching_assistant (std::string course_id, Instructor TA) {
    // Find the course with the given course_id
    learn::course* course = db::database::get_instance ().get_course_by_id (course_id);

    if (course) {
        // Check if the TA is already assigned as a teaching assistant for the course
        if (course->has_teaching_assistant (TA)) {
            // TA is already a teaching assistant for the course, do nothing
            return false;
        }

        // Assign the TA as a teaching assistant for the course
        course->add_teaching_assistant (TA);

        // Update the database with the new teaching assistant assignment for the course
        db::database::get_instance ().add_teaching_assistant_to_course (
        course_id, TA.get_id ());

        return true; // Teaching assistant successfully added to the course
    }

    return false; // Course with course_id not found
}

int instructor::get_maximum_grade (std::string course_id) {
    // Find the course with the given course_id
    learn::course* course = db::database::get_instance ().get_course_by_id (course_id);

    if (course) {
        // Get the maximum grade for the course
        int maxGrade = course->get_maximum_grade ();

        return maxGrade;
    }

    // Return a default value or handle error as needed
    return -1; // Default value indicating course not found or error
}


int instructor::get_minimum_grade (std::string course_id) {
    // Find the course with the given course_id
    learn::course* course = db::database::get_instance ().get_course_by_id (course_id);

    if (course) {
        // Get the minimum grade for the course
        int minGrade = course->get_minimum_grade ();

        return minGrade;
    }

    // Return a default value or handle error as needed
    return -1; // Default value indicating course not found or error
}


double instructor::get_average_grade (std::string course_id) {
    // Find the course with the given course_id
    learn::course* course = db::database::get_instance ().get_course_by_id (course_id);

    if (course) {
        // Get the average grade for the course
        double averageGrade = course->get_average_grade ();

        return averageGrade;
    }

    // Return a default value or handle error as needed
    return -1.0; // Default value indicating course not found or error
}

bool instructor::modify_grade (Student student, Assignment assignment, int new_grade) {
    // Find the student in the database
    auth::student* dbStudent =
    db::database::get_instance ().get_student_by_id (student.get_id ());

    if (dbStudent) {
        // Find the assignment in the database
        learn::assignment* dbAssignment =
        db::database::get_instance ().get_assignment_by_id (assignment.get_id ());

        if (dbAssignment) {
            // Update the grade for the student's assignment in the database
            std::map<std::string, std::any> newProps;
            newProps["grade"] = std::to_string (new_grade);

            if (db::database::get_instance ().update_item (*dbAssignment, newProps)) {
                return true; // Grade successfully modified
            }
        }
    }

    return false; // Student or assignment not found
}






void instructor::get () {
    if (!saved_in_db ())
        throw utils::custom_exception{ "Item not saved in database;" };

    std::string query_string =
    "select users.*, instructors.id as instructor_id, "
    "instructors.is_teaching_assistant, (select "
    "group_concat(instructors_courses.course_id) from instructors_courses "
    "where instructors_courses.instructor_id = instructors.id) as courses from "
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
    return true;
}
bool instructor::remove_from_database (SQLite::Database& db) {
    return true;
}
bool instructor::update_in_database (SQLite::Database& db,
std::map<std::string, std::any> props) {
    return true;
}
} // namespace auth
