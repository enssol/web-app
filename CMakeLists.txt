cmake_minimum_required(VERSION 3.10)

# Project Name and Settings
project(app)

# Set the C Standard and Compiler
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_COMPILER musl-clang)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c90 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -I/usr/local/include -I/usr/local/include/openssl -Wall -ansi -Wextra -pedantic -O3 -ffunction-sections -fdata-sections -fno-common -fstack-protector-strong -static -pg)

# Disable C++ compiler
set(CMAKE_CXX_COMPILER "")

# Enable position-independent code (PIC)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Enable compilation database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set static linking flags
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)

# Add source files recursively
file(GLOB_RECURSE SOURCE_FILES "src/*.c" "src/*.s")

# Optionally, you can print the list of source files for debugging
# message(STATUS "Source files: ${SOURCE_FILES}")

# Add executable target
add_executable(enveng_web_app ${SOURCE_FILES})

# Link libraries
target_link_libraries(app PRIVATE pthread m recutils)

# Ensure Ninja is used as the generator
if (NOT CMAKE_GENERATOR STREQUAL "Ninja")
    message(FATAL_ERROR "Please use Ninja as the generator: cmake -G Ninja .")
endif()

# Packaging
include(CPack)
