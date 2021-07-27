#pragma once
#include <iostream>


class ServerNetwork {
public:
    ServerNetwork();
    void Initialize(void *,unsigned short);
    void Listen();
    //void ConnectClients();
    //void DisconnectClient();
    void Run();
};


