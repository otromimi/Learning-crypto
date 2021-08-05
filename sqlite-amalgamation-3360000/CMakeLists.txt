# CMakeLists file for sqlite3

cmake_minimum_required(VERSION 3.8)

project(sqlite3)

# Compiling library
add_library(sqlite3 sqlite3.c)

# Building sqlite shell 
add_executable(sqlite3-shell shell.c)
target_link_libraries(sqlite3-shell PUBLIC sqlite3)