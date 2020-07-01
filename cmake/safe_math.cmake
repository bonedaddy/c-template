macro(define_safe_math)
    add_library(safemathstatic
        STATIC
            ./include/utils/safe_math.h
            ./src/utils/safe_math.c
    )
    add_executable(safe-math-test ./src/utils/safe_math_test.c)
    target_compile_options(safe-math-test PRIVATE ${flags})
    target_link_libraries(safe-math-test cmocka)
    target_link_libraries(safe-math-test safemathstatic)
    target_compile_options(safe-mem-test PRIVATE ${flags})
    add_test(NAME SafeMathTest COMMAND safe-math-test)
endmacro()