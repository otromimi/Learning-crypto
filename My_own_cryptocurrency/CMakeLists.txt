# CMakeList.txt : CMake project for My_own_cryptocurrency, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(My_crypto)

set(SOURCE My_own_cryptocurrency.cpp Wallet.h Wallet.cpp DB_operations.h Node.h Structures.h)
add_executable(${PROJECT_NAME} ${SOURCE})

set_target_properties(${PROJECT_NAME} PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
            )

# target_include_directories must be added AFTER add_executable
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/cryptopp850/)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/struct_mapping-master/include/struct_mapping/)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/sqlite-amalgamation-3360000/)

# linking against libraries
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/cryptopp850/libcryptopp.a)

target_link_libraries(${PROJECT_NAME} ${CMAKE_BINARY_DIR}/sqlite-amalgamation-3360000/libsqlite3.a)


# TODO: Add tests and install targets if needed.
message(STATUS "Makefiles for ${PROJECT_NAME} done.")
