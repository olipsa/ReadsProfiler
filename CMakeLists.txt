cmake_minimum_required(VERSION 3.19)
project(untitled2)

set(CMAKE_CXX_STANDARD 14)

set(PROJECT_HEADERS
        Server/Server.h
        Server/ServerNetwork.h
        )
set(PROJECT_SOURCES
        Server/main.cpp
        Server/Server.cpp
        Server/ServerNetwork.cpp
        )
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(untitled2 ${PROJECT_SOURCES} ${PROJECT_HEADERS})