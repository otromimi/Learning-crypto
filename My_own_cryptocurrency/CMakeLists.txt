# CMakeList.txt : CMake project for My_own_cryptocurrency, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(My_crypto)

set(SOURCE My_own_cryptocurrency.cpp)
add_executable(${PROJECT_NAME} ${SOURCE})

# target_include_directories must be added AFTER add_executable
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/cryptopp850/)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/sqlite-amalgamation-3360000/)

# linking against libraries
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/cryptopp850/libcryptopp.a)

target_link_libraries(${PROJECT_NAME} ${CMAKE_BINARY_DIR}/sqlite-amalgamation-3360000/libsqlite3.a)


# TODO: Add tests and install targets if needed.
message(STATUS "Makefiles for ${PROJECT_NAME} done.")
