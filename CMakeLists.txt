cmake_minimum_required(VERSION 3.14)
project(ElepthicPoint)

set (CMAKE_CXX_FLAGS "-fexceptions -Wall")

set(CMAKE_CXX_STANDARD 17)

enable_testing()
SET(BUILD_TESTING true)

if(${BUILD_TESTING})
    message("Build the testing tree.")
    add_subdirectory(Test)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/scripts")
include(DirectorySeacher)


include_directories(RSA ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(RSA ${CMAKE_CURRENT_SOURCE_DIR}/BigInt)
include_directories(RSA ${CMAKE_CURRENT_SOURCE_DIR}/Utilits)

add_dir_to_executable(RSA ${CMAKE_CURRENT_SOURCE_DIR})
dir_to_target_sources(RSA ${CMAKE_CURRENT_SOURCE_DIR}/BigInt)
dir_to_target_sources(RSA ${CMAKE_CURRENT_SOURCE_DIR}/Utilits)
