#include "auth/administrator.h"
#include "db/database.h"
#include "learn/course_registration.h"

#include <iostream>
namespace auth {

administrator::administrator (std::string name, std::string faculty, std::string email, std::string password)
: user (name, faculty, email, password, Role::ADMINISTRATOR) {
}

// Get from database
// Raise utils::custom_exception("Administrator not found") if not there
administrator::administrator (std::string id) : user (id) {
    get ();
}

utils::vector<std::unique_ptr<administrator>> administrator::getAdministrators (
std::map<std::string, std::string> props) {
    std::string query_string =
    "select administrators.id from administrators join "
    "users on users.id == administrators.user_id";

    utils::vector<std::unique_ptr<administrator>> results;

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
        std::string administrator_id = (std::string)query.getColumn (0);
        std::unique_ptr<administrator> s (new administrator (administrator_id));
        results.push_back (std::move (s));
    }

    return results;
}

void administrator::get () {
    if (!saved_in_db ())
        throw utils::custom_exception{ "Item not saved in database;" };

    std::string query_string =
    "select users.*, administrators.id from administrators join users on "
    "users.id == administrators.user_id where administrators.id = ?";

    SQLite::Statement query (db::database::get_db (), query_string);
    query.bind (1, _id);

    if (!query.executeStep ())
        throw utils::custom_exception ("Invalid Id for administrator");
    _password_hash          = (std::string)query.getColumn (1);
    _email                  = (std::string)query.getColumn (2);
    _faculty                = (std::string)query.getColumn (3);
    _name                   = (std::string)query.getColumn (4);
    std::string role_string = (std::string)query.getColumn (5);
    _role                   = string_to_role (role_string);

#if PRINT_DATA_WHEN_RETRIEVED
    std::cout << "Administrator: " << _name << "\nID: " << _id << "\nEmail: " << _email
              << "\nFaculty: " << _faculty << "\nRole: " << role_string << std::endl
              << std::endl;
#endif
}
bool administrator::add_to_database (SQLite::Database& db) {
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

    query = SQLite::Statement (db, "INSERT INTO Administrators(id, user_id) VALUES(?,?)");
    query.bind (1, id);
    query.bind (2, user_id);

    int success = query.exec ();
    _id         = id;
    return success;
}
bool administrator::remove_from_database (SQLite::Database& db) {
    SQLite::Statement query (db,
    "DELETE FROM Users WHERE Users.id = (SELECT Users.id FROM Users JOIN "
    "Administrators ON Administrators.user_id == Users.id WHERE "
    "Administrators.id = ?)");
    query.bindNoCopy (1, _id);

    int success = query.exec ();
    return success;
}

bool administrator::update_in_database (SQLite::Database& db,
std::map<std::string, std::any> props) {
    std::string query_string = "UPDATE Users SET ";
    int count                = 0;
    for (auto const& [key, val] : props) {
        if (count > 0)
            query_string += ",";
        query_string += key + " = '" + std::any_cast<std::string> (val) + "'";
        count++;
    }

    query_string += " WHERE id = (SELECT Users.id FROM Users JOIN "
                    "Administrators ON Administrators.user_id == Users.id "
                    "WHERE Administrators.id = ?)";

    SQLite::Statement query (db, query_string);
    query.bindNoCopy (1, _id);

    int success = query.exec ();
    return success;
}

utils::vector<std::unique_ptr<learn::course_registration>>
administrator::list_pending_registrations () {
    // TODO DB: return all course registrations that returns all the registrations that have the same faculty
}


bool administrator::register_course_for_student (learn::course_registration& course_reg) {
    return db::database::get_instance ().update_item (
    course_reg, { { "state"s, "Enrolled"s } });
}

bool administrator::register_course_for_student (std::string course_id, std::string student_id) {
    // TODO DB: Find if there are a course registration
    // If there's any, change its state to Enrolled
    // If not, use the following commands
    learn::course_registration reg (course_id, student_id);
    return register_course_for_student (reg);
}


bool administrator::add_course (learn::course& course) {
    db::database::get_instance ().add_item (course);
    return db::database::get_instance ().update_item (course, { { "faculty", _faculty } });
}


bool administrator::assign_course (learn::course& course, auth::instructor& instructor) {
    SQLite::Statement query (db::database::get_db (),
    "insert into Instructors_Courses (instructor_id, course_id) values(? , ?)");
    query.bind (1, instructor.get_id ());
    query.bind (2, course.get_id ());

    int success = query.exec ();

    return success;
}
} // namespace auth
