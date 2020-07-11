macro(define_colors_lib)
    add_library(colorslib
        SHARED
            ./include/utils/colors.h
            ./src/utils/colors.c
    )
    target_compile_options(colorslib PRIVATE ${flags})
endmacro()