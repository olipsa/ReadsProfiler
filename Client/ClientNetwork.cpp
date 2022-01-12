//
// Created by ondina on 27.07.2021.
//

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
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
    cout<<"create-socket\n";
    client_sd=socket(AF_INET,SOCK_STREAM,0);
    if(client_sd<0){
        cout<<"Error with socket client"<<endl;
        exit(1);
    }

    cout<<"Socket client has been created"<<endl;


}
void ClientNetwork::connect(const char *address, int portNum) {
    cout<<"connect\n";
    client_addr.sin_family=AF_INET;
    client_addr.sin_addr.s_addr=inet_addr(address);
    client_addr.sin_port=htons(portNum);
    cout<<41<<endl;
    if (::connect(client_sd,(struct sockaddr *)&client_addr, sizeof(client_addr)) == -1){
        cout<<"Connection to the server failed\n";
        exit(0);
    }
    cout<<46<<endl;
    cout<<"Connected to the server"<<endl;
    isConnected=true;
    cout<<49<<endl;

    char *buffer;
    cout<<52<<endl;
    while(isConnected){
        cout<<"Enter your message: ";
        cout<<55<<endl;
        fflush(stdout);
        cout<<57<<endl;
        buffer= static_cast<char *>(malloc(1024));
        cin.getline(buffer,1024);
        cout<<59<<endl;
        send_length(strlen(buffer));
        cout<<61<<endl;
        send(buffer);
        int length=receive_length();
        receive(length);

    }

    close(client_sd);

}
void ClientNetwork::send(char *buffer) {
    cout<<"send\n";
    if(write(client_sd,buffer,strlen(buffer))==-1){
        cout<<"Error when sending message\n";
        exit(0);
    }
}
void ClientNetwork::send_length(int length) {
    cout<<"send_length\n";
    if(write(client_sd,&length,sizeof(length))==-1){
        cout<<"Error when sending message\n";
        exit(0);
    }
}
void ClientNetwork::send_file(const char * file_path) {
    cout<<"send_file\n";
    cout<<file_path<<endl;
    FILE *book, *book_copy;
    book = fopen("divergent.pdf", "rb");
    if(book == NULL)
        cout<<"Unable to open pdf.\n";
    book_copy = fopen("9780743477116.pdf", "wb");
    if(book_copy == NULL)
        cout<<"Unable to open pdf copy.\n";
    ifstream in("divergent.pdf", ios::binary);
    if(in.fail())
    {
        cout<<"\nThe file couldn't be opened\n";
        exit(0);
    }
    int *buf;
    while(!feof(book)){
        cout<<"0";
        fread(buf,1,1,book);
        cout<<"1";
        fwrite(buf,1,1,book_copy);
        cout<<"2";
    }
    cout<<"File copied successfully.\n";
    fclose(book);
    fclose(book_copy);

}
void ClientNetwork::receive(int length) {
    cout<<"receive\n";
    string message;
    message.resize(length);
    if(read(client_sd, &message[0],length)==-1){
        cout<<"Error when receiving message\n";
        exit(0);
    }
    if(message == "Exited program..."){
        isConnected=false;
    }
    cout<<message.c_str()<<"\n";
    if(message == "Start sending file."){
        send_file("romeo-and-juliet.pdf");
    }

}
int ClientNetwork::receive_length() {
    cout<<"receive_length";
    int length;
    if(read(client_sd,&length,sizeof(int))==-1){
        cout<<"Error when receiving length\n";
        return -1;
    }
    return length;

}
