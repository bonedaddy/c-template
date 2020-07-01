# sets minimum version
cmake_minimum_required(VERSION 3.0)

# project name and language
project(c-template LANGUAGES C)

# include macros
include(cmake/safe_mem.cmake)
include(cmake/safe_math.cmake)
include(cmake/command_line.cmake)
include(cmake/cli.cmake)

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

# add custom libraries
add_library(randomshared
    SHARED
        ./include/utils/random.h
        ./src/utils/random.c
)


add_library(loggershared
    SHARED
        ./include/utils/logger.h
        ./src/utils/logger.c
)

# set library compile options
target_compile_options(randomshared PRIVATE ${flags})

# defines executables to build
add_executable(logger-test ./src/utils/logger.c)
add_executable(array-len-test ./src/utils/array_test.c)
add_executable(random-test ./src/utils/random_test.c)

# set executable compiler options
target_compile_options(logger-test PRIVATE ${flags})
target_compile_options(array-len-test PRIVATE ${flags})
target_compile_options(random-test PRIVATE ${flags})


# links libraries
target_link_libraries(logger-test cmocka)
target_link_libraries(logger-test zlog)
target_link_libraries(array-len-test cmocka)
target_link_libraries(random-test cmocka)
target_link_libraries(random-test randomshared)

# execute macros
define_safe_mem()
define_safe_math()
define_command_line()
define_c_template_REPLACEME()

# enable testing
enable_testing()
add_test(NAME ArrayLenTest COMMAND array-len-test)
add_test(NAME RandomTest COMMAND random-test)
