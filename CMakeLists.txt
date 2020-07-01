# sets minimum version
cmake_minimum_required(VERSION 3.0)

# project name and language
project(c-template LANGUAGES C)

# include macros 
include(cmake/safe_mem_lib.cmake)
include(cmake/safe_mem_test.cmake)
include(cmake/safe_math_lib.cmake)
include(cmake/safe_math_test.cmake)
include(cmake/command_line_lib.cmake)
include(cmake/command_line_test.cmake)
include(cmake/cli.cmake)
include(cmake/random_lib.cmake)
include(cmake/random_test.cmake)
include(cmake/logger_lib.cmake)
include(cmake/logger_test.cmake)
include(cmake/array_len_test.cmake)

# setup build flags
list(APPEND flags -std=c17 -Wall -Wextra -Werror -pedantic)

# TODO: the following causes this to fail
# set release type
#if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
# endif()
# print build type
# message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# print flags used
message(STATUS "C flags, Debug Configuration: ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "C flags, Release Configuration: ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "C flags, Release configuration with Debug info: ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message(STATUS "C flags, minimal Release configuration: ${CMAKE_C_FLAGS_MINSIZEREL}")

# build project cli
define_cli()

# build safe memory library
define_safe_mem_lib()
# comment following line to disable test builds
define_safe_mem_test()

# build safe math library
define_safe_math_lib()
# comment following line to disable test builds
define_safe_math_test()

# build command line lib
define_command_line_lib()
# uncomment following line to disable test builds
define_command_line_test()

# build random generation library
define_random_lib()
# comment following line to disable test builds
define_random_test()

# build logger library
define_logger_lib()
# comment following line to disable test builds
define_logger_test()

# test array header functionality
# comment following line to disable test builds
define_array_len_test()

# enable testing
enable_testing()


