#pragma once
#include <iostream>

typedef struct thData{
    int idThread;
    int cl;
}thData;
class ServerNetwork {
public:
    ServerNetwork();
    ~ServerNetwork();
    void Initialize(void *,unsigned short);
    void Listen();
    //void ConnectClients();
    //void DisconnectClient();
    void Run();
    static void *Treat(void*);
    void Answer(void*);
    void Send(char*,thData);
    void Receive(std::string&,thData);
    int IsValid(std::string);
    //void ServeClient(Command command);

};


