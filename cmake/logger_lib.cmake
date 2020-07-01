macro(define_logger_lib)
    add_library(loggerlib
        SHARED
        ./include/utils/logger.h
        ./src/utils/logger.c
    )
    target_compile_options(loggerlib PRIVATE ${flags})
endmacro()