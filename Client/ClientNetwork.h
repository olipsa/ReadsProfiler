//
// Created by ondina on 27.07.2021.
//
#pragma once
#include <string>
using namespace std;

class ClientNetwork {
    bool isConnected=false;
public:
    ClientNetwork(void *);
    void create_socket();
    void connect(const char *, int );
    void send(char[]);
    void send_length(int);
    void send_file(const char *);
    void receive(int);
    int receive_length();

};



