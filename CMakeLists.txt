cmake_minimum_required(VERSION 2.8)

project(breakable_scope)
add_executable(${PROJECT_NAME} "test/test.cpp")

INCLUDE_DIRECTORIES("include" "test")
