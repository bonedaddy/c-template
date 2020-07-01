macro(define_safe_mem_lib)
    add_library(safememshared
        SHARED
            ./include/utils/safe_mem.h
            ./src/utils/safe_mem.c
    )
    target_compile_options(safememshared PRIVATE ${flags})
endmacro()