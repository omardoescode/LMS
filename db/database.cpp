#include "db/database.h"

#include "SQLiteCpp/Database.h"

#include <iostream>
#include <string_view>
namespace db {
database::database() : _db(SQLite::Database("example.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE)) {
};
void database::initialize_db() {
    _db.exec("CREATE TABLE IF NOT EXISTS Users (user_id INTEGER PRIMARY KEY, password_hash VARCHAR(32), email VARCHAR(255), faculty VARCHAR(255), name VARCHAR(255))");
    _db.exec("CREATE TABLE IF NOT EXISTS Students (student_id VARCHAR(9) PRIMARY KEY, user_id INTEGER, FOREIGN KEY(user_id) REFERENCES Users(user_id))");
    _db.exec("CREATE TABLE IF NOT EXISTS Students_Courses (student_id VARCHAR(9), course_id INTEGER, state TEXT)");
    _db.exec("CREATE TABLE IF NOT EXISTS Instructors (instructor_id VARCHAR(255) PRIMARY KEY, is_teaching_assistant BOOL, user_id INTEGER, FOREIGN KEY(user_id) REFERENCES Users(id))");
    _db.exec("CREATE TABLE IF NOT EXISTS Instructors_Courses (instructor_id VARCHAR(255), course_id INTEGER)");
    _db.exec("CREATE TABLE IF NOT EXISTS Administrators (administrator_id VARCHAR(255) PRIMARY KEY, user_id INTEGER, FOREIGN KEY(user_id) REFERENCES Users(id))");
    _db.exec("CREATE TABLE IF NOT EXISTS Courses (course_id INTEGER PRIMARY KEY, name VARCHAR(255), credit_hours INTEGER, text_book VARCHAR(255))");

    /*
    _db.exec ("INSERT INTO Users(password_hash, email) VALUES('test', 'test@rars.c')");
    std::cout << _db.getLastInsertRowid ();
    std::string x = _db.execAndGet ("INSERT INTO Students(student_id, user_id) VALUES('23-101287', '" + std::to_string (_db.getLastInsertRowid ()) + "')");
    std::cout << _db.getLastInsertRowid ();
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

        std::cout << "Course: " << name << "\nID: " << id << "\nCredit Hours: " << credits << "\nTextbook: " << textbook << std::endl << std::endl;
    }
    */

    /*
    SQLite::Statement query(_db, "INSERT INTO Users(name, password_hash, email, faculty) VALUES(?, ?, ?, ?)");
    query.bindNoCopy(1, "moha09");
    query.bindNoCopy(2, "asfasfasfa");
    query.bindNoCopy(3, "test@email.com");
    query.bindNoCopy(4, _faculty);

    int user_id = query.executeStep();


    std::string student_id = "23-101283";

    SQLite::Statement query2(db, "INSERT INTO Students(student_id, user_id) VALUES(?, ?)");
    query2.bind(1, student_id);
    query2.bind(2, user_id);

    int success = query2.exec();
    _id = student_id;
    return success > 0;
    */
}
} // namespace db
