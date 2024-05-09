cmake_minimum_required(VERSION 3.5)
project(lsm)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(libraries SQLiteCpp sqlite3 pthread)
set(sources
        auth/administrator.cpp
        auth/administrator.h
        auth/instructor.cpp
        auth/instructor.h
        auth/user.cpp
        auth/user.h
        auth/student.cpp
        auth/student.h
        learn/assignment.cpp
        learn/assignment.h
        learn/assignment_submission.cpp
        learn/assignment_submission.h
        learn/course.cpp
        learn/course.h
        learn/course_registration.cpp
        learn/course_registration.h
        db/database.cpp
        db/database.h
        db/database_item.h
        utils/exceptions.h)
add_executable(main main.cpp ${sources}
        auth/role.h)
add_executable(tests tests/tests.cpp ${sources})

target_include_directories(main PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(main ${libraries})
target_link_libraries(tests ${libraries})
