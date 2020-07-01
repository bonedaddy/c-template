macro(define_safe_math_lib)
    add_library(safemathshared
        SHARED
            ./include/utils/safe_math.h
            ./src/utils/safe_math.c
    )
    target_compile_options(safemathshared PRIVATE ${flags})
endmacro()