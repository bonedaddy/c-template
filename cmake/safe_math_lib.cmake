macro(define_safe_math_lib)
    add_library(safemathlib
        SHARED
            ./include/utils/safe_math.h
            ./src/utils/safe_math.c
    )
    target_compile_options(safemathlib PRIVATE ${flags})
endmacro()