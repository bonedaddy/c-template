# sets minimum version
cmake_minimum_required(VERSION 3.0)

# project name and language
project(c-template LANGUAGES C)

include(cmake/FetchCMocka.cmake)

add_library(safememstatic
    STATIC
        ./include/utils/safe_mem.h
        ./src/utils/safe_mem.c
)
add_library(randomstatic
    SHARED
        ./include/utils/random.h
        ./src/utils/random.c
)
add_library(safemathstatic
    STATIC
        ./include/utils/safe_math.h
        ./src/utils/safe_math.c
)

# defines executables to build
add_executable(array-len ./src/utils/array_test.c)
add_executable(random ./src/utils/random_test.c)
add_executable(safe-math ./src/utils/safe_math_test.c)
add_executable(safe-mem ./src/utils/safe_mem_test.c)


# links libraries
target_link_libraries(array-len cmocka)

target_link_libraries(random cmocka)
target_link_libraries(random randomstatic)

target_link_libraries(safe-math cmocka)
target_link_libraries(safe-math safemathstatic)

target_link_libraries(safe-mem cmocka)
target_link_libraries(safe-mem safememstatic)