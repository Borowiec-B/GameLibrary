cmake_minimum_required(VERSION 3.5)

project(GameLibrary)

include("${CMAKE_SOURCE_DIR}/cmake/utilities.cmake")


set(main_target		GameLibrary)

set(include_dir	"${CMAKE_SOURCE_DIR}/include/")
set(source_dir	"${CMAKE_SOURCE_DIR}/src/")

append_prefixed_items_to_list("${source_dir}GameLibrary/" source_files main.cpp)


add_library(${main_target} STATIC ${source_files})

set_target_properties(${main_target} PROPERTIES CXX_STANDARD 17
									 			CXX_STANDARD_REQUIRED TRUE
												CXX_EXTENSIONS FALSE
)


# Perform all tests after build, abort producing the library on failure.
#
# CMakeLists.txt executed by "add_subdirectory()" creates test targets and sets "test_target".
# Executables for testing are placed in "${CMAKE_BINARY_DIR}/test/".
add_subdirectory("${CMAKE_SOURCE_DIR}/test/")
add_custom_command(	TARGET				${main_target}
					COMMAND				"${test_target}"
					COMMENT				"Run all tests."
					WORKING_DIRECTORY	"test/"
					POST_BUILD
)
