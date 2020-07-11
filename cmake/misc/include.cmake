macro(include_cmake_files)

    include(cmake/pkgs/FindSphinx.cmake)
    include(cmake/pkgs/UseSphinxDoc.cmake)

    include(cmake/pkgs/doxygen.cmake)

    include(cmake/libraries/wait_group_lib.cmake)
    include(cmake/tests/wait_group_test.cmake)

    include(cmake/tests/socket_test.cmake)
    include(cmake/libraries/socket_lib.cmake)

    include(cmake/pkgs/glib.cmake)

    include(cmake/tests/returns_test.cmake)
    include(cmake/libraries/returns_lib.cmake)

    include(cmake/libraries/errors_lib.cmake)
    include(cmake/tests/errors_test.cmake)

    include(cmake/libraries/colors_lib.cmake)
    include(cmake/tests/colors_test.cmake)

    include(cmake/pkgs/setup_valgrind.cmake)

    include(cmake/libraries/safe_mem_lib.cmake)
    include(cmake/tests/safe_mem_test.cmake)

    include(cmake/libraries/safe_math_lib.cmake)
    include(cmake/tests/safe_math_test.cmake)

    include(cmake/libraries/command_line_lib.cmake)
    include(cmake/tests/command_line_test.cmake)

    include(cmake/misc/cli.cmake)

    include(cmake/libraries/random_lib.cmake)
    include(cmake/tests/random_test.cmake)

    include(cmake/libraries/logger_lib.cmake)
    include(cmake/tests/logger_test.cmake)

    include(cmake/libraries/arrays_lib.cmake)
    include(cmake/tests/array_len_test.cmake)

endmacro()