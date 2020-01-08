cmake_minimum_required(VERSION 3.13)
project(toplevel)

enable_testing()

# Set install directory if not set from outside
if(NOT CMAKE_INSTALL_PREFIX OR CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/Release" CACHE PATH "..." FORCE)
endif()

add_subdirectory(3rdParty)
add_subdirectory(src)
add_subdirectory(tests)
