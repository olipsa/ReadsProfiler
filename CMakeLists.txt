cmake_minimum_required(VERSION 3.19)
project(untitled2)

set(CMAKE_CXX_STANDARD 14)

set(PROJECT_HEADERS_SERVER
        Server/Server.h
        Server/ServerNetwork.h
        Server/Database.h
        Server/Commands/Command.h
        Server/Commands/Register.h
        Server/Commands/Login.h
        Server/Commands/Search.h
        Server/Commands/Upload.h
        Server/Commands/Download.h)

set(PROJECT_SOURCES_SERVER
        Server/main.cpp
        Server/Server.cpp
        Server/ServerNetwork.cpp
        Server/Database.cpp
        Server/Commands/Command.cpp
        Server/Commands/Register.cpp
        Server/Commands/Login.cpp
        Server/Commands/Search.cpp
        Server/Commands/Upload.cpp
        Server/Commands/Download.cpp)

set(PROJECT_HEADERS_CLIENT
        Client/Client.h
        Client/ClientNetwork.h)

set(PROJECT_SOURCES_CLIENT
        Client/main.cpp
        Client/Client.cpp
        Client/ClientNetwork.cpp)

find_package(SQLite3)


SET(CMAKE_CXX_FLAGS "-lsqlite3 -pthread")

add_executable(server ${PROJECT_SOURCES_SERVER} ${PROJECT_HEADERS_SERVER})
add_executable(client ${PROJECT_SOURCES_CLIENT} ${PROJECT_HEADERS_CLIENT})

if (SQLITE3_FOUND)
    include_directories(${SQLITE3_INCLUDE_DIRS})
    target_link_libraries (server ${SQLITE3_LIBRARIES})
endif (SQLITE3_FOUND)
include_directories(/usr/include)
link_directories(/usr/lib)
target_link_libraries(server sqlite3)


