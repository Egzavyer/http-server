cmake_minimum_required(VERSION 3.28)
project(http_server)

set(CMAKE_CXX_STANDARD 23)

add_executable(http_server main.cpp
        client/client.cpp
        client/client.h
        server/server.cpp
        server/server.h)
