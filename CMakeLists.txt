cmake_minimum_required(VERSION 3.5)
project(lsm)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp)
set(CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(libraries SQLiteCpp sqlite3 pthread)
set(sources
        learn/assignment.cpp
        learn/assignment.h
        learn/assignment_submission.cpp
        learn/assignment_submission.h
        learn/course.cpp
        learn/course.h
        learn/course_registration.h
        learn/course_registration.cpp
        auth/administrator.cpp
        auth/administrator.h
        auth/instructor.cpp
        auth/instructor.h
        auth/user.cpp
        auth/user.h
        auth/student.cpp
        auth/student.h
        db/database.cpp
        db/database.h
        db/database_item.h
        utils/exceptions.h
        utils/timer.h
        utils/vector.h
        utils/split_string.h)
add_executable(main main.cpp ${sources})
add_executable(tests tests/tests.cpp ${sources})
add_executable(seeder database_setup.cpp db/database.h db/database.cpp)

target_include_directories(main PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(seeder PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(main PUBLIC ${libraries})
target_link_libraries(tests PUBLIC ${libraries})
target_link_libraries(seeder PUBLIC ${libraries})

target_precompile_headers(main PUBLIC pch.h)
target_precompile_headers(tests PUBLIC pch.h)
target_precompile_headers(seeder PUBLIC pch.h) 
