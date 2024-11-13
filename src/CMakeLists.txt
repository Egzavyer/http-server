set(SOURCES
        main.cpp
        WinsockNetworking.cpp
        LinuxNetworking.cpp
        Server.cpp
        ConnectionHandler.cpp
        ../include/ConnectionHandler.h
)
add_executable(server ${SOURCES})

target_link_libraries(server PRIVATE ws2_32)