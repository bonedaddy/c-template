# sets minimum version
cmake_minimum_required(VERSION 3.0)

# project name and language
project(c-template LANGUAGES C)

# include macros
include(cmake/safe_mem_lib.cmake)
include(cmake/safe_mem_test.cmake)
include(cmake/safe_math_lib.cmake)
include(cmake/safe_math_test.cmake)
include(cmake/logger_lib.cmake)
include(cmake/command_line.cmake)
include(cmake/cli.cmake)
include(cmake/random_lib.cmake)
include(cmake/random_test.cmake)
include(cmake/logger_lib.cmake)
include(cmake/logger_test.cmake)

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


# defines executables to build
add_executable(array-len-test ./src/utils/array_test.c)


# set executable compiler options
target_compile_options(array-len-test PRIVATE ${flags})

# links libraries
target_link_libraries(array-len-test cmocka)

# execute macros
define_safe_mem_lib()
define_safe_mem_test()
define_safe_math_lib()
define_safe_math_test()
define_command_line()
define_c_template_REPLACEME()
define_random_lib()
define_random_test()
define_logger_lib()
define_logger_test()

# enable testing
enable_testing()
add_test(NAME ArrayLenTest COMMAND array-len-test)

