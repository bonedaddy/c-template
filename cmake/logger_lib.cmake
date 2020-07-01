macro(define_logger_lib)
    add_library(loggershared
        SHARED
        ./include/utils/logger.h
        ./src/utils/logger.c
    )
    target_compile_options(loggershared PRIVATE ${flags})
endmacro()