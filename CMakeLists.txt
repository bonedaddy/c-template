#####################
# BEGIN CMAKE SETUP #
#####################

# sets minimum version
cmake_minimum_required(VERSION 3.0)

project(c-template-REPLACEME LANGUAGES C)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # -g indicates to include debug symbols
    list(APPEND flags -std=c17 -Wall -Wextra -Werror -pedantic -g)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    list(APPEND flags -std=c17 -Wall -Wextra -Werror -pedantic)
endif()

# needed for building socket stuff
list(APPEND flags -D_POSIX_C_SOURCE=201112L)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/libraries")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/misc")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/pkgs")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/tests")

###################
# END CMAKE SETUP #
###################

include(cmake/misc/include.cmake)
include_cmake_files()



######################
# END MACRO INCLUDES #
######################

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build flags: ${flags}")

###############################
# BEGIN MACRO FUNCTION INVOKE #
###############################

# to disable test builds comment out `*_test()` lines

define_wait_group_lib()
define_wait_group_test()

define_socket_test()
define_socket_lib()

define_returns_test()
define_returns_lib()

define_errors_lib()
define_errors_test()

define_colors_lib()
define_colors_test()

define_setup_valgrind()

define_cli()

define_safe_mem_lib()
define_safe_mem_test()

define_safe_math_lib()
define_safe_math_test()

define_command_line_lib()
define_command_line_test()

define_random_lib()
define_random_test()

define_logger_lib()
define_logger_test()
 
define_arrays_lib()
define_array_len_test()

add_doxygen_doc(
    BUILD_DIR
        ${PROJECT_SOURCE_DIR}/build
    DOXY_FILE
        ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in
    TARGET_NAME
        doxygen-docs
    COMMENT
        "HTML documentation"
)

add_sphinx_doc(
  SOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/docs
  BUILD_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/_build
  CACHE_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/_doctrees
  HTML_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/sphinx_html
  CONF_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/docs/conf.py
  TARGET_NAME
    sphinx-docs
  COMMENT
    "HTML documentation"
)

#############################
# END MACRO FUNCTION INVOKE #
#############################

################
# ENABLE CTEST #
################

enable_testing()


