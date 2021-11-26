//
// Created by ondina on 19.07.2021.
//

#include "Server.h"
Server::Server() {}
Server::~Server() {}
void Server::Start() {
    database.OpenDatabase("main.db");
    database.ExecuteQuery("CREATE TABLE IF NOT EXISTS users(username VARCHAR(16) PRIMARY KEY, "
                          "password VARCHAR(32))");

    database.ExecuteQuery("CREATE TABLE IF NOT EXISTS books("
                              "book_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "title VARCHAR(16), "
                              "author VARCHAR(32), "
                              "year INTEGER, "
                              "ISBN INTEGER, "
                              "rating REAL)");

    database.ExecuteQuery("CREATE TABLE IF NOT EXISTS book_genres("
                              "book_id INTEGER, "
                              "genre VARCHAR(16), "
                              "CONSTRAINT fk_book_id "
                                "FOREIGN KEY(book_id) "
                                "REFERENCES books(book_id),"
                              "PRIMARY KEY(book_id,genre))");

    database.ExecuteQuery("CREATE TABLE IF NOT EXISTS author_genres("
                              "author VARCHAR(16), "
                              "genre VARCHAR(16), "
                              "PRIMARY KEY(author,genre))");
    server_network.Initialize(this,8088);
    server_network.Listen();
    server_network.Run();
    database.CloseDatabase();
}
Database Server::getDatabase() {
    return database;
}
