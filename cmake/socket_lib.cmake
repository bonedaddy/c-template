macro(define_socket_lib)
    add_library(socketlib
        SHARED
            ./include/network/socket/socket.h
            ./include/network/socket/socket_server.h
            ./src/network/socket/socket.c
            ./src/network/socket/socket_server.c
    )
    target_compile_options(socketlib PRIVATE ${flags})
    target_link_libraries(socketlib loggerlib)
    target_link_libraries(socketlib waitgrouplib)
endmacro()