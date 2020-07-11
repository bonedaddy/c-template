macro(define_arrays_lib)
    add_library(arrayslib
        SHARED
            ./include/utils/arrays.h
            ./src/utils/arrays.c
    )
    target_compile_options(arrayslib PRIVATE ${flags})
    target_link_libraries(arrayslib pthread)
endmacro()