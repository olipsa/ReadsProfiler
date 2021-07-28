cmake_minimum_required(VERSION 3.19)
project(untitled2)

set(CMAKE_CXX_STANDARD 14)

set(PROJECT_HEADERS_SERVER
        Server/Server.h
        Server/ServerNetwork.h)
set(PROJECT_SOURCES_SERVER
        Server/main.cpp
        Server/Server.cpp
        Server/ServerNetwork.cpp)

SET(CMAKE_CXX_FLAGS -pthread)

add_executable(server ${PROJECT_SOURCES_SERVER} ${PROJECT_HEADERS_SERVER})

set(PROJECT_HEADERS_CLIENT
        Client/Client.h
        Client/ClientNetwork.h)
set(PROJECT_SOURCES_CLIENT
        Client/main.cpp
        Client/Client.cpp
        Client/ClientNetwork.cpp)

add_executable(client ${PROJECT_SOURCES_CLIENT} ${PROJECT_HEADERS_CLIENT})
