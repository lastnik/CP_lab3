cmake_minimum_required(VERSION 3.14)
project(ElepthicPoint)

set (CMAKE_CXX_FLAGS "-fexceptions -Wall")

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../scripts")
include(DirectorySeacher)


set(GTEST "C:/GoogleTest")
include_directories(Test ${GTEST}/googletest/include)
include_directories(Test ${GTEST}/googlemock/include)

include_directories(Test ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(Test ${CMAKE_CURRENT_SOURCE_DIR}/BigInt)
include_directories(Test ${CMAKE_CURRENT_SOURCE_DIR}/Utilits)
add_dir_to_executable(Test ${CMAKE_CURRENT_SOURCE_DIR})
dir_to_target_sources(Test ${CMAKE_CURRENT_SOURCE_DIR}/BigInt)
dir_to_target_sources(Test ${CMAKE_CURRENT_SOURCE_DIR}/Utilits)

target_link_libraries(Test ${GTEST}/build/lib/libgtest.a ${GTEST}/build/lib/libgtest_main.a ${GTEST}/build/lib/libgmock.a ${GTEST}/build/lib/libgmock_main.a)