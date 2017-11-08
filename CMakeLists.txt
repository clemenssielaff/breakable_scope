cmake_minimum_required(VERSION 2.8)

project(breakable_scope)
add_executable(${PROJECT_NAME} "test/test.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
INCLUDE_DIRECTORIES("include" "test")
