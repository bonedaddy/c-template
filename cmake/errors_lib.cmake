macro(define_errors_lib)
    add_library(errorslib
        SHARED
            ./include/utils/errors.h
            ./src/utils/errors.c
    )
    add_executable(errors-test ./src/utils/errors.c)
    target_compile_options(errorslib PRIVATE ${flags})
    target_link_libraries(errors-test errorslib)
endmacro()