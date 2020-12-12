cmake_minimum_required(VERSION 3.5)

project(GameLibraryTest LANGUAGES CXX)


set(test_target GameLibraryTest)
set(test_target ${test_target} PARENT_SCOPE)


find_package(Catch2 REQUIRED)

add_executable(${test_target} main.cpp)
target_link_libraries(${test_target} PRIVATE Catch2::Catch2)

include(CTest)
include(Catch)

catch_discover_tests(${test_target})
