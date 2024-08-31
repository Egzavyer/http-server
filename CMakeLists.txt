cmake_minimum_required(VERSION 3.28)
project(http_server)

set(CMAKE_CXX_STANDARD 23)

add_executable(ServerExecutable
        server/server.cpp
        common/common.cpp
        server/connection.cpp
        server/httpRequest.cpp
        server/httpRequest.h
        server/httpParser.cpp
        server/httpParser.h
)

add_executable(ClientExecutable
        client/client.cpp
        common/common.cpp
)

target_link_libraries(ServerExecutable PRIVATE ws2_32)

target_link_libraries(ClientExecutable PRIVATE ws2_32)