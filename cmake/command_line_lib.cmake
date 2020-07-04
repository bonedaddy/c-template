macro(define_command_line_lib)
    add_library(commandlinelib
        SHARED
            ./include/utils/command_line.h
            ./src/utils/command_line.c
    )
    target_link_libraries(commandlinelib colorslib)
    target_link_libraries(commandlinelib argtable3)
    target_link_libraries(commandlinelib m)
    target_compile_options(commandlinelib PRIVATE ${flags})
endmacro()