macro(define_safe_mem_lib)
    add_library(safememlib
        SHARED
            ./include/utils/safe_mem.h
            ./src/utils/safe_mem.c
    )
    target_compile_options(safememlib PRIVATE ${flags})
endmacro()