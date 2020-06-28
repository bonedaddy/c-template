# sets minimum version
cmake_minimum_required(VERSION 3.0)

# project name and language
project(c-template LANGUAGES C)

# this doesnt appear to be needed unless
# we want to have user not be required to install cmake
# include(cmake/FetchCMocka.cmake)

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
add_library(safememstatic
    STATIC
        ./include/utils/safe_mem.h
        ./src/utils/safe_mem.c
)
add_library(randomshared
    SHARED
        ./include/utils/random.h
        ./src/utils/random.c
)
add_library(safemathstatic
    STATIC
        ./include/utils/safe_math.h
        ./src/utils/safe_math.c
)

# set library compile options
target_compile_options(safememstatic PRIVATE ${flags})
target_compile_options(randomshared PRIVATE ${flags})
target_compile_options(safemathstatic PRIVATE ${flags})


# defines executables to build
add_executable(array-len-test ./src/utils/array_test.c)
add_executable(random-test ./src/utils/random_test.c)
add_executable(safe-math-test ./src/utils/safe_math_test.c)
add_executable(safe-mem-test ./src/utils/safe_mem_test.c)

# set executable compiler options
target_compile_options(array-len-test PRIVATE ${flags})
target_compile_options(random-test PRIVATE ${flags})
target_compile_options(safe-math-test PRIVATE ${flags})
target_compile_options(safe-mem-test PRIVATE ${flags})

# links libraries
target_link_libraries(array-len-test cmocka)
target_link_libraries(random-test cmocka)
target_link_libraries(random-test randomshared)
target_link_libraries(safe-math-test cmocka)
target_link_libraries(safe-math-test safemathstatic)
target_link_libraries(safe-mem-test cmocka)
target_link_libraries(safe-mem-test safememstatic)


# enable testing
enable_testing()
add_test(NAME ArrayLenTest COMMAND array-len-test)
add_test(NAME RandomTest COMMAND random-test)
add_test(NAME SafeMathTest COMMAND safe-math-test)
add_test(NAME SafeMemTest COMMAND safe-mem-test)