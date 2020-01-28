cmake_minimum_required(VERSION 3.13)

# Set variables
set(TARGET_NAME DataRepositoryTests)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/) # DLL and EXE dir
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/) # Shared libs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/) # Static libs

message(STATUS "Target: ${TARGET_NAME}")
message(STATUS "Installation to: ${CMAKE_INSTALL_PREFIX}")

project(${TARGET_NAME})

# Define target content
add_executable(${TARGET_NAME}
    TestMain.cpp
	ResultSetTest.cpp
	DataRepositoryTest.cpp
    UserRepoTest.cpp
)
add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})

# Define dependent libraries
target_link_libraries(${TARGET_NAME} DAL)
target_link_libraries(${TARGET_NAME} DomainObjects)
target_link_libraries(${TARGET_NAME} Persistance)
target_link_libraries(${TARGET_NAME} gtest)
target_link_libraries(${TARGET_NAME} gmock)

# define target properties
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)

# install executable
install(TARGETS ${TARGET_NAME}
	DESTINATION ${CMAKE_INSTALL_PREFIX})
