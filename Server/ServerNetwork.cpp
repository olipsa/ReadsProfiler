//
// Created by ondina on 19.07.2021.
//
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include <cstring>
#include "ServerNetwork.h"
#include "Server.h"
#include "Commands/Register.h"
#include "Commands/Login.h"
#include "Commands/Search.h"
#include "Commands/Upload.h"
#include "Commands/Download.h"
#include "Commands/View.h"
#include <algorithm>
#include <string>

using namespace std;
typedef void * (*THREADFUNCPTR)(void *);

Server *server;
int server_sd;//file descriptor containing the values returned by the socket system call
struct sockaddr_in server_addr; //structure containing an internet address, in out case the address of our client
struct sockaddr_in from; //structure containing an internet address, in out case the address of our client
unsigned short portNum;
vector<int>connected_threads;
int ServerNetwork::number_of_threads=0;


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
        td->idThread=++number_of_threads;
        td->cl=client;
        cout<<"Client #"<<td->idThread<<" connected\n";
        cout<<"Connected threads:";
        for(int i=0;i<connected_threads.size();i++)
            cout<<connected_threads[i]<<" ";
        cout<<endl;
        pthread_create(&threads[number_of_threads],NULL,(THREADFUNCPTR)&Treat,td);
    }
}
void* ServerNetwork::Treat(void* arg) //function executed for each client
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
    while(true) {
        int length_of_message;
        if (!ReceiveLength(length_of_message,tdL))
            break;
        string command;
        if(!Receive(command, length_of_message, tdL))
            break;
        string message_to_send;
        int return_code = IsValid(command, message_to_send, tdL);
        if (return_code == 0){
            message_to_send = "Command not known";
        }
        else if (return_code == -1) { //exit command received
            message_to_send = "Exited program...";
            SendLength(message_to_send.length(),tdL);
            Send(const_cast<char *>(message_to_send.c_str()), tdL);
            break;
        }

        SendLength(message_to_send.length(),tdL);
        Send(const_cast<char *>(message_to_send.c_str()), tdL);
        if(strcmp(message_to_send.c_str(),"Start sending file.") == 0)
        {
            cout<<"File received\n";
        }

    }
    cout<<"Client #"<<tdL.idThread<<" disconnected."<<endl;
    number_of_threads--;
    for(int i=0;i<connected_threads.size();i++)
        if(connected_threads[i]==tdL.idThread){
            connected_threads.erase(connected_threads.begin()+i);
            break;
        }
    cout<<"Connected threads: "<<number_of_threads<<endl;
}
bool ServerNetwork::Receive(string &buffer,int size,thData tdL){

    buffer.resize(size);
    if (read (tdL.cl, &buffer[0],size) <= 0)
    {
        cout<<"Thread "<<tdL.idThread<<endl;
        perror ("Error when reading from client");
        return 0;

    }
    cout<<"Message received from thread "<<tdL.idThread<<": \""<<buffer.c_str()<<"\""<<endl;
    buffer.resize(strlen(buffer.c_str()));
    return 1;

}
bool ServerNetwork::ReceiveLength(int &length,thData tdL){
    if (read(tdL.cl, &length,sizeof(int)) <= 0)
    {
        cout<<"Thread "<<tdL.idThread<<endl;
        perror("Error when reading from client");
        return 0;
    }
    return 1;
}
bool ServerNetwork::Send(char * message,thData tdL){
    if (write (tdL.cl, message,strlen(message)) <= 0)
    {
        cout<<"Thread "<<tdL.idThread<<endl;
        perror("Error when writing to client");
        return 0;
    }
    else
        cout<<"Message sent to thread "<<tdL.idThread<<endl;
    return 1;
}
bool ServerNetwork::SendLength(int length,thData tdL){
    if (write (tdL.cl, &length,sizeof(length)) <= 0)
    {
        cout<<"Thread "<<tdL.idThread<<endl;
        perror("Error when writing to client");
        return 0;
    }
    return 1;
}

int ServerNetwork::IsValid(string buffer, string& message,thData tdL) {
    vector<string> argument_list;
    string command;
    ParseCommand(buffer,argument_list, command);

    //find command to execute
    if(command=="exit")
        return -1;

    if(command=="register"){
        Register registerCommand(argument_list,server);
        message = registerCommand.Execute();
        return 1;
    }
    else if(command=="login"){
        if(!count(connected_threads.begin(),connected_threads.end(),tdL.idThread))
        {
            Login loginCommand(argument_list,server);
            message = loginCommand.Execute();
            if(message.find("Logged in with success.")==0)
                connected_threads.push_back(tdL.idThread);
        }
        else
            message = "You are already connected.";
        return 1;
    }
    else if(!count(connected_threads.begin(),connected_threads.end(),tdL.idThread)){
        message = "You must log in first.";
        return 1;
    }
    else if(command=="search"){
        Search searchCommand(argument_list,server);
        message = searchCommand.Execute();
        tdL.last_search_result = searchCommand.get_search_result();
        return 1;
    }
    else if(command =="upload"){
        Upload uploadCommand(argument_list,server);
        if(uploadCommand.BookFound(argument_list[0]))
            message = "Start sending file.";
        else
            message = "Book not found.";
        cout<<message<<endl;
        return 1;

    }
    else if(command=="download"){
        Download downloadCommand(argument_list,server);
        message = downloadCommand.Execute();
        return 1;
    }
    else if(command =="view"){

    }

    return 0;
}
void ServerNetwork::ParseCommand(string buffer,vector<string>& argument_list, string &command){
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
    command=argument_list[0];
    transform(command.begin(),command.end(),command.begin(),[](unsigned char c){ return std::tolower(c); });
    argument_list.erase(argument_list.begin());
}


