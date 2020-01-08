cmake_minimum_required(VERSION 3.13)
include(GenerateExportHeader)

# Set variables
set(TARGET_NAME Persistance)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/) # DLL and EXE dir
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/) # Shared libs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/) # Static libs

message(STATUS "Target: ${TARGET_NAME}")
message(STATUS "Installation to: ${CMAKE_INSTALL_PREFIX}")

project(${TARGET_NAME})

# Define target content
add_library(${TARGET_NAME} SHARED
	src/UserRepo.cpp
	src/DataRepository.cpp
)

# Define headers for this library. 
# PUBLIC headers will be added to consumers' build paths and will be used for local build.
target_include_directories(${TARGET_NAME} 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Define dependent libraries
target_link_libraries(${TARGET_NAME} DomainObjects)
target_link_libraries(${TARGET_NAME} DAL)

# define target properties
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)

# install targets
install(TARGETS ${TARGET_NAME}
	DESTINATION ${CMAKE_INSTALL_PREFIX})

# This makes the project importable from the build directory
export(TARGETS ${TARGET_NAME} FILE ${TARGET_NAME}Config.cmake)

# Add Unit Tests
#[[add_executable(${TARGET_NAME}Test
    test/test.cpp)

target_link_libraries(${TARGET_NAME}Test
    ${TARGET_NAME})

add_test(NAME ${TARGET_NAME}Test COMMAND ${TARGET_NAME}Test)
]]