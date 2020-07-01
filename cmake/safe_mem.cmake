macro(define_safe_mem)
    add_library(safememstatic
        STATIC
            ./include/utils/safe_mem.h
            ./src/utils/safe_mem.c
    )
    add_executable(safe-mem-test ./src/utils/safe_mem_test.c)
    target_compile_options(safememstatic PRIVATE ${flags})
    target_link_libraries(safe-mem-test cmocka)
    target_link_libraries(safe-mem-test safememstatic)
    target_compile_options(safe-mem-test PRIVATE ${flags})
    add_test(NAME SafeMemTest COMMAND safe-mem-test)
endmacro()