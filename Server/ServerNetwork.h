#pragma once
#include <iostream>
#include <vector>
using namespace std;

typedef struct thData{
    int idThread;
    int cl;
    string connected_user="";
    vector<string> last_search_result;
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
    bool Send(char*,thData);
    bool SendLength(int,thData);
    bool Receive(string&,int,thData);
    bool ReceiveLength(int&,thData);
    bool ReceiveFile(thData);
    int IsValid(string,string&,thData&);
    void ParseCommand(string,vector<string>&, string&);
    //void ServeClient(Command command);

};


