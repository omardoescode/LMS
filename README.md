# Running the project

1. First things first, make sure you have CMake Installed
2. Run the following command for the first time in the project directory

```
mkdir build/
cmake -S . -B ./build
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

**NOTE**: For CLion users, Clion auto-handles the entire process of running
cmake and building prior to running the program
