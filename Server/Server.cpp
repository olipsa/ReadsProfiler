//
// Created by ondina on 19.07.2021.
//

#include "Server.h"
Server::Server() {}
Server::~Server() {}
void Server::Start() {
    database.OpenDatabase("/home/ondina/CLionProjects/ReadsProfiler-website-crawler/main.db");
    database.ExecuteQuery("CREATE TABLE IF NOT EXISTS users(username VARCHAR(16) PRIMARY KEY, "
                          "password VARCHAR(32))");
    database.ExecuteQuery("CREATE TABLE IF NOT EXISTS user_activity(username VARCHAR(16) PRIMARY KEY, "
                          "searches TEXT, views TEXT, downloads TEXT)");
    server_network.Initialize(this,8088);
    server_network.Listen();
    server_network.Run();
    database.CloseDatabase();
}
Database Server::getDatabase() {
    return database;
}
