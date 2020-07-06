macro(define_returns_lib)
    add_library(returnslib
        SHARED
            ./include/utils/returns.h
            ./src/utils/returns.c
    )
    target_compile_options(returnslib PRIVATE ${flags})
    target_link_libraries(returnslib errorslib)
endmacro()