macro(define_command_line)
    add_library(commandlineshared
        SHARED
            ./include/utils/command_line.h
            ./src/utils/command_line.c
    )
    add_executable(command-line-test ./src/utils/command_line_test.c)
    target_link_libraries(command-line-test cmocka)
    target_link_libraries(command-line-test commandlineshared)
    target_link_libraries(command-line-test safememstatic)
    target_link_libraries(command-line-test loggershared)
    target_link_libraries(command-line-test zlog)
    target_compile_options(command-line-test PRIVATE ${flags})
    target_compile_options(commandlineshared PRIVATE ${flags})
    add_test(NAME CommandLineTest COMMAND command-line-test)
endmacro()