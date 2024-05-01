cmake_minimum_required(VERSION 3.5)
project(lsm)

set(sources learning/learning.cpp learning/learning.h auth/auth.h auth/auth.cpp
            db/db.h db/db.cpp)
add_executable(main main.cpp ${sources})
add_executable(tests tests/tests.cpp ${sources})

target_include_directories(main PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR})
