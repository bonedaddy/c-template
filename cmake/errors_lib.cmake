macro(define_errors_lib)
    add_library(errorslib
        SHARED
            ./include/utils/errors.h
            ./src/utils/errors.c
    )
    target_compile_options(errorslib PRIVATE ${flags})
endmacro()