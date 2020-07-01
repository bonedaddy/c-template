macro(define_command_line_lib)
    add_library(commandlinelib
        SHARED
            ./include/utils/command_line.h
            ./src/utils/command_line.c
    )
    target_compile_options(commandlinelib PRIVATE ${flags})
endmacro()