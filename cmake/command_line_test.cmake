macro(define_command_line_test)
    add_executable(command-line-test ./src/utils/command_line_test.c)
    target_link_libraries(command-line-test cmocka)
    target_link_libraries(command-line-test commandlinelib)
    target_link_libraries(command-line-test safememlib)
    target_link_libraries(command-line-test loggerlib)
    target_link_libraries(command-line-test zlog)
    target_link_libraries(commandlinelib argtable3)
    target_link_libraries(commandlinelib m)
    target_compile_options(command-line-test PRIVATE ${flags})
    add_test(NAME CommandLineTest COMMAND command-line-test)
endmacro()