cmake_minimum_required(VERSION 3.5)

set(version 0.1.0)

project(GameLibrary VERSION ${version} LANGUAGES CXX)

include("${CMAKE_SOURCE_DIR}/cmake/utilities.cmake")

find_package(Boost REQUIRED)

set(main_target	GameLibrary)

set(include_dir	"${CMAKE_SOURCE_DIR}/include")
set(source_dir	"${CMAKE_SOURCE_DIR}/src")

append_prefixed_items_to_list("${source_dir}/GameLibrary/" source_files main.cpp)
append_prefixed_items_to_list("${source_dir}/GameLibrary/Console/" source_files Command.cpp Console.cpp Cvar.cpp)
append_prefixed_items_to_list("${source_dir}/GameLibrary/Event/" source_files Dispatcher.cpp)
append_prefixed_items_to_list("${source_dir}/GameLibrary/Utilities/" source_files String.cpp)


add_library(${main_target} STATIC ${source_files})

set_target_properties(${main_target} PROPERTIES CXX_STANDARD 17
									 			CXX_STANDARD_REQUIRED TRUE
												CXX_EXTENSIONS FALSE
)

# ./src/ may contain headers for internal usage.
target_include_directories(${main_target} PUBLIC ${include_dir} PRIVATE {source_dir} PUBLIC ${Boost_INCLUDE_DIRS})


# This will always perform all tests after build.
#
# Executables for testing are placed in "${CMAKE_BINARY_DIR}/test/".
add_subdirectory("${CMAKE_SOURCE_DIR}/test/")

