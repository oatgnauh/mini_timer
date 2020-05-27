cmake_minimum_required(VERSION 3.7.0)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main 
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/timer.cpp
)