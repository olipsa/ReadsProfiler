//
// Created by ondina on 27.07.2021.
//

#include "Client.h"
Client::Client(){}
void Client::Run(){
    client_network=new ClientNetwork(this);
    client_network->create_socket();
    client_network->connect("127.0.0.1",8088);
};
