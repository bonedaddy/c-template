# defines and compiles the random header
macro(define_random_lib)
    add_library(randomshared
        SHARED
            ./include/utils/random.h
            ./src/utils/random.c
    )
    target_compile_options(randomshared PRIVATE ${flags})
endmacro()