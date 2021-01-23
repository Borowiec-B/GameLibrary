cmake_minimum_required(VERSION 3.5)

include("${CMAKE_SOURCE_DIR}/cmake/utilities.cmake")


set(test_target GameLibraryTest)
set(test_target ${test_target} PARENT_SCOPE)
set(test_source_dir ${CMAKE_CURRENT_SOURCE_DIR})

append_prefixed_items_to_list("${test_source_dir}/" test_source_files main.cpp)
append_prefixed_items_to_list("${test_source_dir}/Console/" test_source_files Cvar.cpp)
append_prefixed_items_to_list("${test_source_dir}/Utilities/" test_source_files	IdManager.cpp Limits.cpp String.cpp Traits.cpp Conversions/String.cpp
																				Conversions/Arithmetic.cpp Conversions/ArithmeticString.cpp)


find_package(Catch2 REQUIRED)

add_executable(${test_target} ${test_source_files})

set_target_properties(${test_target} PROPERTIES CXX_STANDARD 17
									 			CXX_STANDARD_REQUIRED TRUE
												CXX_EXTENSIONS FALSE
)

target_link_libraries(${test_target} PRIVATE Catch2::Catch2 PRIVATE ${main_target})

include(CTest)
include(Catch)

catch_discover_tests(${test_target})

add_custom_target(	RunAllTests
					ALL
					COMMAND				"${test_target}"
					COMMENT				"Run all tests."
					DEPENDS				"${test_target}"
)

