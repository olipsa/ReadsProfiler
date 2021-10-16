//
// Created by ondina on 19.07.2021.
//

#include "Server.h"
Server::Server() {}
Server::~Server() {}
void Server::Start() {
    database.OpenDatabase("main.db");
    database.ExecuteQuery("CREATE TABLE IF NOT EXISTS users (username VARCHAR(16) PRIMARY KEY, password VARCHAR(32))");
    std::vector<std::string> results;
    database.GetQueryResults("SELECT * FROM users WHERE username=\"on\"",results);
    server_network.Initialize(this,8088);
    server_network.Listen();
    server_network.Run();
    database.CloseDatabase();
}
Database Server::getDatabase() {
    return database;
}
