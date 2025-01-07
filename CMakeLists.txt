# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Tema2 VERSION 1.0)

# Specify the C++ standard (optional)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Recursively find all .cpp files in the src/ directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add an executable target
add_executable(client.exe client.cpp ${SOURCES})
add_executable(server.exe server.cpp ${SOURCES})

# Include directories (if needed)
target_include_directories(client.exe PUBLIC include)
target_include_directories(server.exe PUBLIC include)

# Link libraries (if needed)
target_link_libraries(client.exe PRIVATE sqlite3)
target_link_libraries(server.exe PRIVATE sqlite3)


