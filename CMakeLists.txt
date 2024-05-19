cmake_minimum_required(VERSION 3.5)
project(lsm)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp)
set(CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(libraries SQLiteCpp sqlite3 pthread)
set(headers 
        learn/assignment_submission.h
        learn/assignment.h
        learn/course.h
        learn/course_registration.h
        auth/login_manager.h
        auth/session.h
        auth/administrator.h
        auth/instructor.h
        auth/user.h
        auth/student.h
        db/database.h
        db/database_item.h
        utils/exceptions.h
        utils/timer.h
        utils/vector.h
        utils/split_string.h
  )
set(sources
        learn/assignment.cpp
        learn/assignment_submission.cpp
        learn/course.cpp
        learn/course_registration.cpp
        auth/login_manager.cpp
        auth/session.cpp
        auth/administrator.cpp
        auth/instructor.cpp
        auth/user.cpp
        auth/student.cpp
        db/database.cpp
        )
set(targets main tests)

 
add_library(lms ${sources} ${headers})
add_executable(tests tests/tests.cpp ${sources} ${headers})
add_executable(seeder database_setup.cpp db/database.h db/database.cpp)

target_include_directories(lms PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(seeder PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(tests PUBLIC ${libraries})
target_link_libraries(lms PUBLIC ${libraries})
target_link_libraries(seeder PUBLIC ${libraries})

target_precompile_headers(lms PUBLIC pch.h)
target_precompile_headers(tests PUBLIC pch.h)
target_precompile_headers(seeder PUBLIC pch.h) 


add_executable(main main.cpp)
target_link_libraries(main PUBLIC lms ${libraries}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
  )

target_include_directories(main PUBLIC ${PROJECT_SOURCE_DIR})
