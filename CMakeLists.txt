cmake_minimum_required(VERSION 3.10)

project(Template)

set(CMAKE_C_FLAGS "-Wall -Wpedantic -std=c11")

include(cmake/FetchCMocka.cmake)

include_directories("${PROJECT_SOURCE_DIR}/src")

add_executable(SafeMemTest src/utils/safe_mem_test.c)
add_dependencies(SafeMemTest SafeMemTest)
target_link_libraries(SafeMemTest PRIVATE cmocka)

add_executable(ArrayTest src/utils/array_test.c)
add_dependencies(ArrayTest ArrayTest)
target_link_libraries(ArrayTest PRIVATE cmocka)

enable_testing()
add_test(NAME SafeMemTest COMMAND SafeMemTest)
add_test(NAME ArrayTest COMMAND ArrayTest)