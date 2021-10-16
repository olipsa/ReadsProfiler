//
// Created by ondina on 19.07.2021.
//

#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include <cstring>
#include "ServerNetwork.h"
#include "Server.h"
#include "Register.h"
#include <algorithm>
#include <string>
using namespace std;
typedef void * (*THREADFUNCPTR)(void *);

Server *server;
int server_sd;//file descriptor containing the values returned by the socket system call
struct sockaddr_in server_addr; //structure containing an internet address, in out case the address of our client
struct sockaddr_in from; //structure containing an internet address, in out case the address of our client
unsigned short portNum;

ServerNetwork::ServerNetwork() {}
ServerNetwork::~ServerNetwork() {}

void ServerNetwork::Initialize(void *data, unsigned short port) {
    server=(Server *)data;
    portNum=port;
    if ((server_sd=socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error establishing connection." << endl;
        exit(1);
    }
    int on=1;
    setsockopt(server_sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    bzero(&server_addr,sizeof(server_addr));
    bzero(&from,sizeof(from));
    cout << "Server socket has been created" << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY); //accept any incoming messages
    server_addr.sin_port = htons(portNum);

    //binding socket = assigning an address to a socket
    if (bind(server_sd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) ==1) {
        cout << "Error binding socket" << endl;
        exit(1);
    }
}
void ServerNetwork::Listen() {
    if (::listen(server_sd, 2) == -1) {
        cout << "Error on listen" << endl;
        exit(1);
    }
    cout << "Looking for clients..." << endl;
}
void ServerNetwork::Run() {
    int clientCount = 0;
    int bufsize = 1024;
    char buffer[bufsize]; //buffer used to read characters from the socket connection
    pthread_t threads[100];
    while (1) {
        int client;
        thData *td;
        socklen_t length=sizeof(from);
        cout<<"Listening on port "<<portNum<<endl;
        fflush(stdout);
        if((client=accept(server_sd,(struct sockaddr *) &from,&length))<0){
            perror("Error on accepting client connection.");
            continue;
        }
        td=(struct thData*)malloc(sizeof(struct thData));
        td->idThread=++clientCount;
        td->cl=client;
        cout<<"Client #"<<td->idThread<<" connected\n";
        pthread_create(&threads[clientCount],NULL,(THREADFUNCPTR)&Treat,td);
    }
}
void* ServerNetwork::Treat(void* arg) //function executed by each client
{
    struct thData tdL;
    tdL= *((struct thData*)arg);
    pthread_detach(pthread_self());
    cout<<"Inside thread "<<tdL.idThread<<". Awaiting message..\n";
    ((ServerNetwork *)arg)->Answer((struct thData*)arg);
    close ((intptr_t)arg); //Closing connection with client
    return(NULL);
}
void ServerNetwork::Answer(void *arg){
    struct thData tdL;
    tdL= *((struct thData*)arg);
    fflush(stdout);
    while(true){
        string command;
        command.resize(1024); //allocate 1024 bytes for the received message
        Receive(command,tdL);

        char *message;
        int return_code= IsValid(command); //
        if(return_code==0)
            Send("Command not known",tdL);
        else if(return_code==-1){
            Send("Exited program...",tdL);
            break;
        }
    }
    cout<<"Client #"<<tdL.idThread<<" disconnected."<<endl;

}
void ServerNetwork::Receive(string &buffer,thData tdL){
    buffer.resize(1024);
    if (read (tdL.cl, &buffer[0],1024) <= 0)
    {
        cout<<"Thread "<<tdL.idThread<<endl;
        perror ("Error when reading from the client.\n");

    }
    cout<<"Message received from thread #"<<tdL.idThread<<": \""<<buffer.c_str()<<"\""<<endl;
    buffer.resize(strlen(buffer.c_str()));

}
void ServerNetwork::Send(char * message,thData tdL){
    if (write (tdL.cl, message,strlen(message)) <= 0)
    {
        cout<<"Thread "<<tdL.idThread<<endl;
        perror ("[Thread]Eroare la write() catre client.\n");
    }
    else
        cout<<"Message successfully sent to thread "<<tdL.idThread<<endl;
}
int ServerNetwork::IsValid(std::string buffer) {
    vector<string> argument_list;
    string token,delimiter=" ";
    size_t pos;
    while((pos=buffer.find(delimiter))!=std::string::npos) {
        //while there are spaces left
        token=buffer.substr(0,pos);
        if(!token.empty())
            argument_list.push_back(token);
        buffer.erase(0,pos+1);
    }
    if(!buffer.empty())
        argument_list.push_back(buffer);

    //Remove spaces and lower characters
    string command=argument_list[0];
    transform(command.begin(),command.end(),command.begin(),[](unsigned char c){ return std::tolower(c); });
    cout<<"Parsed command: "<<command<<endl;

    //find command to execute
    if(command=="register"){
        Register registerCommand(argument_list,server);
        cout<<registerCommand.Execute();
        return 1; //server should only return
    }
    else if(command=="exit")
        return -1;

    return 0;
}


