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

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

###################
# END CMAKE SETUP #
###################

##################
# BEGIN INCLUDES #
##################

include(cmake/UseBreathe.cmake)

include(cmake/FindSphinx.cmake)
include(cmake/UseSphinxDoc.cmake)

include(cmake/doxygen.cmake)

include(cmake/glib.cmake)

include(cmake/returns_test.cmake)
include(cmake/returns_lib.cmake)

include(cmake/errors_lib.cmake)
include(cmake/errors_test.cmake)

include(cmake/colors_lib.cmake)
include(cmake/colors_test.cmake)

include(cmake/setup_valgrind.cmake)

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

################
# END INCLUDES #
################

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build flags: ${flags}")

###############################
# BEGIN MACRO FUNCTION INVOKE #
###############################

# to disable test builds comment out `*_test()` lines

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
 
define_array_len_test()

add_doxygen_doc(
    BUILD_DIR
        ${PROJECT_SOURCE_DIR}/_build
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

add_breathe_doc(
  SOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/docs
  BUILD_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/_build
  CACHE_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/_doctrees
  HTML_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/html
  DOXY_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in
  CONF_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/docs/conf.py
  TARGET_NAME
    breathe-docs
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


