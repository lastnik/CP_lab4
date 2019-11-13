cmake_minimum_required(VERSION 3.14)
project(ECDH)

set (CMAKE_CXX_FLAGS "-fexceptions -Wall")

set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/scripts")
include(DirectorySeacher)


include_directories(ECDH ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(ECDH ${CMAKE_CURRENT_SOURCE_DIR}/BigInt)
include_directories(ECDH ${CMAKE_CURRENT_SOURCE_DIR}/Utilits)
include_directories(ECDH ${CMAKE_CURRENT_SOURCE_DIR}/ElipticCurve)
include_directories(ECDH ${CMAKE_CURRENT_SOURCE_DIR}/Users)

add_dir_to_executable(ECDH ${CMAKE_CURRENT_SOURCE_DIR})
dir_to_target_sources(ECDH ${CMAKE_CURRENT_SOURCE_DIR}/BigInt)
dir_to_target_sources(ECDH ${CMAKE_CURRENT_SOURCE_DIR}/Utilits)
dir_to_target_sources(ECDH ${CMAKE_CURRENT_SOURCE_DIR}/ElipticCurve)
dir_to_target_sources(ECDH ${CMAKE_CURRENT_SOURCE_DIR}/Users)

enable_testing()
SET(BUILD_TESTING true)

if(${BUILD_TESTING})
    message("Build the testing tree.")
    add_subdirectory(Test)
endif()