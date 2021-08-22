# CMakeList.txt : CMake project for My_own_cryptocurrency, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(My_crypto)

set(SOURCE My_own_cryptocurrency.cpp
            DB_operations.cpp
            MerkleTree_sha256.cpp
            Network.cpp
            Node.cpp
            Structures.cpp
            Tools.cpp
            Wallet.cpp
            DB_operations.h
            MerkleTree_sha256.h
            Node.h
            Structures.h
            Tools.h
            Wallet.h )

add_executable(${PROJECT_NAME} ${SOURCE})

set_target_properties(${PROJECT_NAME} PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
            )

# target_include_directories must be added AFTER add_executable
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/cryptopp/)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/struct_mapping/include/struct_mapping/)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/sqlite-amalgamation-3360000/)

# linking against libraries
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/cryptopp/libcryptopp.a)

target_link_libraries(${PROJECT_NAME} ${CMAKE_BINARY_DIR}/sqlite-amalgamation-3360000/libsqlite3.a)


# TODO: Add tests and install targets if needed.
message(STATUS "Makefiles for ${PROJECT_NAME} done.")
