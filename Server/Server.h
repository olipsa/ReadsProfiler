//
// Created by ondina on 19.07.2021.
//
#pragma once

#include "ServerNetwork.h"
#include "Database.h"

class Server {
    Database database;
    ServerNetwork server_network;
public:
    Server();
    ~Server();
    void Start();
    Database getDatabase();
};


