//
// Created by ondina on 27.07.2021.
//

#pragma once
#include "ClientNetwork.h"

class Client {
    ClientNetwork *client_network;
public:
    Client();
    void Run();
};

