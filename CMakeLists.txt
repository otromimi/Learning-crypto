# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("My_own_cryptocurrency")

message(STATUS "Generating makefiles for project and sqlite dependencies,\
cryptocpp dependencies should be build before with its own makefile.")

# Including dependencies compiled by CMake
add_subdirectory ("sqlite-amalgamation-3360000")

# Include sub-projects.
add_subdirectory ("My_own_cryptocurrency")


