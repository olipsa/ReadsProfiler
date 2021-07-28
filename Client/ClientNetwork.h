//
// Created by ondina on 27.07.2021.
//
#ifndef UNTITLED2_CLIENTNETWORK_H
#define UNTITLED2_CLIENTNETWORK_H
#include <string>
using namespace std;

class ClientNetwork {
    bool isConnected=false;
public:
    ClientNetwork(void *);
    void create_socket();
    string getIPAddress();
    void connect(const char *, int );
};
#endif //UNTITLED2_CLIENTNETWORK_H


