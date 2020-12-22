cmake_minimum_required(VERSION 3.5)

include("${CMAKE_SOURCE_DIR}/cmake/utilities.cmake")


set(test_target GameLibraryTest)
set(test_target ${test_target} PARENT_SCOPE)
set(test_source_dir ${CMAKE_CURRENT_SOURCE_DIR})

append_prefixed_items_to_list("${test_source_dir}/" test_source_files main.cpp)


find_package(Catch2 REQUIRED)

add_executable(${test_target} ${test_source_files})

set_target_properties(${main_target} PROPERTIES CXX_STANDARD 17
									 			CXX_STANDARD_REQUIRED TRUE
												CXX_EXTENSIONS FALSE
)

target_include_directories(${test_target} PRIVATE ${include_dir})

target_link_libraries(${test_target} PRIVATE Catch2::Catch2)

include(CTest)
include(Catch)

catch_discover_tests(${test_target})
