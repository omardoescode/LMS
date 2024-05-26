# Team Members

The team is called 'Byte Busters Evolved' (a long story why the name is big and
funny)

Members:

- Omar Mohammad: 23-101288
- Mohammd Mosalamy: 23-101283
- Omar Hesham: 23-101302
- Omar Shafiy: 23-201356

# Intro

Welcome to Athena, A library that provides a complete design of a student
management system. It handles authentication using sessions and loggin ing using
IDs and passwords, learn components like courses, assignments, submissions,
grades, etc...

# Running the project

1. First things first, make sure you have CMake Installed
2. Run the following command for the first time in the project directory

```
mkdir build/
cmake -S . -B ./build  # for UNIX-Based OS like macos or linux
cmake -S . -B ./build -G "MinGW Makefiles"# for Windows users
```

3. To run the project, use the following commands

```
cmake --build ./build --target main
./build/main # for UNIX-Based OS like macos or linux
./build/main.exe # for Windows users
```

4. To run tests

```
cmake --build ./build --target tests
./build/tests # for UNIX-Based OS like macos or linux
./build/tests.exe # for Windows users
```

5. To seed the database

```
cmake --build ./build --target seeder
./build/seeder # for UNIX-Based OS like macos or linux
./build/seeder.exe # for Windows users
```

**NOTE**: If you want your IDE to handle running the program, just make sure
that it runs it from the sources directory and not the build one, for example,
CLION does use the build directory as the current directory and it causes
problems with database behavior
