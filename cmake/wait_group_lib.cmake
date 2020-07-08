macro(define_wait_group_lib)
    add_library(waitgrouplib
        SHARED
            ./include/sync/wait_group.h
            ./src/sync/wait_group.h
    )
    target_compile_options(waitgrouplib PRIVATE ${flags})
    target_link_libraries(waitgrouplib pthread)
encmacro()