# Running the project

1. First things first, make sure you have CMake Installed
2. Run the following command for the first time in the project directory

```
mkdir build/
cmake -S . -B ./build
```

3. To run the project, use the following commands

```
cmake --build ./build
./build/main # for UNIX-Based OS like macos or linux
./build/main.exe # for Windows users
```
