# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("My_own_cryptocurrency")

# Include sub-projects.
add_subdirectory ("My_own_cryptocurrency")


# Including dependencies compiled by CMake
add_subdirectory ("sqlite-amalgamation-3360000")
