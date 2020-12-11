cmake_minimum_required(VERSION 3.5)

project(GameLibrary)


set(main_target GameLibrary)

set(include_dir	"${CMAKE_SOURCE_DIR}/include/")
set(source_dir "${CMAKE_SOURCE_DIR}/src/")
set(source_files "${CMAKE_SOURCE_DIR}/src/GameLibrary/main.cpp")


add_library(${main_target} STATIC ${source_files})

set_target_properties(${main_target} PROPERTIES CXX_STANDARD 20
									 			CXX_STANDARD_REQUIRED TRUE
												CXX_EXTENSIONS FALSE
)

