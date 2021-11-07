//
// Created by ondina on 27.07.2021.
//

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "ClientNetwork.h"
#include "Client.h"

using namespace std;

Client * client_net;
int client_sd;
struct sockaddr_in client_addr;
ClientNetwork::ClientNetwork(void *data) {
    client_net=(Client*)data;
}

void ClientNetwork::create_socket() {
    client_sd=socket(AF_INET,SOCK_STREAM,0);
    if(client_sd<0){
        cout<<"Error with socket client"<<endl;
        exit(1);
    }

    cout<<"Socket client has been created"<<endl;


}
void ClientNetwork::connect(const char *address, int portNum) {
    client_addr.sin_family=AF_INET;
    client_addr.sin_addr.s_addr=inet_addr(address);
    client_addr.sin_port=htons(portNum);
    if (::connect(client_sd,(struct sockaddr *)&client_addr, sizeof(client_addr)) == -1){
        cout<<"Connection to the server failed\n";
        exit(0);
    }
    cout<<"Connected to the server"<<endl;
    isConnected=true;

    char *buffer;
    while(isConnected){
        cout<<"Enter your message: ";
        fflush(stdout);
        cin.getline(buffer,1024);
        send_length(strlen(buffer));
        send(buffer);
        int length=receive_length();
        receive(length);

    }

    close(client_sd);

}
void ClientNetwork::send(char *buffer) {
    if(write(client_sd,buffer,strlen(buffer))==-1){
        cout<<"Error when sending message\n";
        exit(0);
    }
    cout<<"Message sent successfully\n";
}
void ClientNetwork::send_length(int length) {
    if(write(client_sd,&length,sizeof(length))==-1){
        cout<<"Error when sending message\n";
        exit(0);
    }
}
void ClientNetwork::receive(int length) {
    string message;
    message.resize(length);
    if(read(client_sd, &message[0],length)==-1){
        cout<<"Error when receiving message\n";
        exit(0);
    }
    if(message == "Exited program..."){
        isConnected=false;
    }
    cout<<"Received message is: \""<<message.c_str()<<"\"\n";

}
int ClientNetwork::receive_length() {
    int length;
    if(read(client_sd,&length,sizeof(int))==-1){
        cout<<"Error when receiving length\n";
        return -1;
    }
    return length;

}
