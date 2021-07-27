//
// Created by ondina on 19.07.2021.
//

#include "Server.h"
Server::Server() {}
void Server::Start() {
    server_network.Initialize(this,8088);
    server_network.Listen();
    server_network.Run();
}
