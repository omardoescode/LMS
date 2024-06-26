#include "db/database.h"
#include "SQLiteCpp/Database.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>
namespace db {
database::database () {
    initialize_db ();
};
void database::refresh_and_seed_db () {
    // Refresh database structure by deleting all existing tables, if any, and
    // creating new tables.
    std::cout << "REFRESHING DATABASE STRUCTURE" << std::endl;
    db::database::get_db ().exec (
    "PRAGMA writable_schema = 1;delete from sqlite_master where type in "
    "('table', 'index', 'trigger');PRAGMA writable_schema = "
    "0;VACUUM;PRAGMA INTEGRITY_CHECK;PRAGMA foreign_keys = ON;");
    initialize_db ();

    // Seed database
    // TODO: MAKE IT RUN IN DEBUG MODE ONLY AND MAKE IT RUN ONCE ONLY AS LONG AS
    // NO CHANGES HAPPEN TO TABLE STRUCTURE
    // TODO: READ THESE CONSTANTS FROM A CONSTATS FILE
    const int NUM_TABLES           = 9;
    std::string tables[NUM_TABLES] = { "Users", "Students", "Instructors",
        "Administrators", "Courses", "Instructors_Courses", "Students_Courses",
        "Assignments", "AssignmentSubmissions" };

    std::cout << "SEEDING DATABASE P.S. THIS MAY TAKE A WHILE" << std::endl;
    for (int i = 0; i < NUM_TABLES; i++) {
        std::ifstream ifs (std::filesystem::current_path ().string () +
        "/db/SeedingData/" + tables[i] + ".sql");
        std::string seed_query ((std::istreambuf_iterator<char> (ifs)),
        (std::istreambuf_iterator<char> ()));
        db::database::get_db ().exec (seed_query);
        std::cout << (i + 1) << " out of " << NUM_TABLES << " Table(s) done." << std::endl;
        ifs.close ();
    }
    std::cout << "DATABASE SEEDED SUCCESSFULLY" << std::endl;
}

void database::initialize_db () {

    std::cout << "INITIALIZING DATABASE" << std::endl;
    db::database::get_db ().exec ("PRAGMA foreign_keys = ON;");
    // Create all tables as per database design
    db::database::get_db ().exec (
    "CREATE TABLE IF NOT EXISTS Users (id INTEGER PRIMARY KEY, "
    "password_hash VARCHAR(32), email VARCHAR(255), faculty "
    "VARCHAR(255), name VARCHAR(255), role TEXT)");
    db::database::get_db ().exec (
    "CREATE TABLE IF NOT EXISTS Students (id VARCHAR(9) PRIMARY KEY, "
    "user_id INTEGER, FOREIGN KEY(user_id) REFERENCES Users(id) ON DELETE "
    "CASCADE)");
    db::database::get_db ().exec ("CREATE TABLE IF NOT EXISTS Students_Courses "
                                  "(id INTEGER PRIMARY KEY, student_id "
                                  "VARCHAR(9), course_id INTEGER, state TEXT)");
    db::database::get_db ().exec (
    "CREATE TABLE IF NOT EXISTS Instructors (id VARCHAR(255) "
    "PRIMARY KEY, is_teaching_assistant BOOL, user_id INTEGER, FOREIGN "
    "KEY(user_id) REFERENCES Users(id) ON DELETE CASCADE)");
    db::database::get_db ().exec (
    "CREATE TABLE IF NOT EXISTS Instructors_Courses (id INTEGER PRIMARY KEY, "
    "instructor_id "
    "VARCHAR(255), course_id INTEGER)");
    db::database::get_db ().exec (
    "CREATE TABLE IF NOT EXISTS Administrators (id "
    "VARCHAR(255) PRIMARY KEY, user_id INTEGER, FOREIGN KEY(user_id) "
    "REFERENCES Users(id) ON DELETE CASCADE)");
    db::database::get_db ().exec (
    "CREATE TABLE IF NOT EXISTS Courses (id INTEGER PRIMARY KEY, "
    "name VARCHAR(255), credit_hours INTEGER, text_book VARCHAR(255), "
    "course_code VARCHAR(255))");
    db::database::get_db ().exec (
    "CREATE TABLE IF NOT EXISTS Assignments (id INTEGER PRIMARY KEY, name "
    "VARCHAR(255), type VARCHAR(255), start_date INTEGER "
    "DEFAULT(strftime('%s', 'now')), due_date INTEGER, available_until_date "
    "INTEGER, course_id INTEGER, max_grade REAL, FOREIGN KEY (course_id) "
    "REFERENCES Courses(id) ON DELETE CASCADE)");
    db::database::get_db ().exec (
    "CREATE TABLE IF NOT EXISTS AssignmentSubmissions (id INTEGER PRIMARY KEY, "
    "grade REAL DEFAULT -1, submission_date INTEGER DEFAULT(strftime('%s', "
    "'now')), assignment_id INTEGER, student_id VARCHAR(9), FOREIGN KEY "
    "(assignment_id) REFERENCES Assignments(id) ON DELETE CASCADE, FOREIGN KEY "
    "(student_id) "
    "REFERENCES Students(id) ON DELETE CASCADE)");

    /*
    db::database::get_db ().exec ("INSERT INTO Users(password_hash, email)
    VALUES('test', 'test@rars.c')"); std::cout << db::database::get_db
    ().getLastInsertRowid (); std::string x = db::database::get_db ().execAndGet
    ("INSERT INTO Students(student_id, user_id) VALUES('23-101287',
    '" + std::to_string (db::database::get_db ().getLastInsertRowid ()) + "')");
    std::cout << db::database::get_db ().getLastInsertRowid ();
    */

    /*
    SQLite::Statement query(_db, "select * from courses where course_id = ?");
    int inp;
    std::cout << "Enter course ID: ";
    std::cin >> inp;
    query.bind(1, inp);
    while(query.executeStep ()) {
        int id = query.getColumn (0);
        std::string name = query.getColumn (1);
        int credits = query.getColumn (2);
        std::string textbook = query.getColumn (3);

        std::cout << "Course: " << name << "\nID: " << id << "\nCredit Hours: "
    << credits << "\nTextbook: " << textbook << std::endl << std::endl;
    }
    */

    /*
    SQLite::Statement query(_db, "INSERT INTO Users(name, password_hash, email,
    faculty) VALUES(?, ?, ?, ?)"); query.bindNoCopy(1, "moha09");
    query.bindNoCopy(2, "asfasfasfa");
    query.bindNoCopy(3, "test@email.com");
    query.bindNoCopy(4, _faculty);

    int user_id = query.executeStep();


    std::string student_id = "23-101283";

    SQLite::Statement query2(db, "INSERT INTO Students(student_id, user_id)
    VALUES(?, ?)"); query2.bind(1, student_id); query2.bind(2, user_id);

    int success = query2.exec();
    _id = student_id;
    return success > 0;
    */
}

SQLite::Database database::_db ("example.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

} // namespace db
