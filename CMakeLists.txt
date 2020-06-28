cmake_minimum_required(VERSION 3.10)

project(Template)

include(cmake/FetchCMocka.cmake)

add_executable(Template src/main.c)

enable_testing()