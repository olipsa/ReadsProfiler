#pragma once
#include <iostream>
#include <vector>
using namespace std;

typedef struct thData{
    int idThread;
    int cl;
}thData;
class ServerNetwork {
    pthread_t threads[100];
    static int number_of_threads;
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
    void SendLength(int,thData);
    void Receive(string&,int,thData);
    void ReceiveLength(int&,thData);
    int IsValid(string,string&);
    void ParseCommand(string,vector<string>&, string&);
    //void ServeClient(Command command);

};


