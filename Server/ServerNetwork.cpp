//
// Created by ondina on 19.07.2021.
//

#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include "ServerNetwork.h"
#include "Server.h"
using namespace std;

Server *server;
int client_sd; //file descriptor containing the values returned by the socket system call
struct sockaddr_in server_addr; //structure containing an internet address, in out case the address of our client
struct sockaddr_in from; //structure containing an internet address, in out case the address of our client
unsigned short portNum;
typedef struct thData{
    int idThread;
    int cl;
}thData;
void answer(void *arg){
    int nr, i=0;
    struct thData tdL;
    tdL= *((struct thData*)arg);
    if (read (tdL.cl, &nr,sizeof(int)) <= 0)
    {
        cout<<"Thread "<<tdL.idThread<<endl;
        perror ("Error when reading from the client.\n");

    }
    cout<<"Message received from thread #"<<tdL.idThread<<endl;

    /*pregatim mesajul de raspuns */
    nr++;
    cout<<"Sending message back\n";

    /* returnam mesajul clientului */
    if (write (tdL.cl, &nr, sizeof(int)) <= 0)
    {
        cout<<"Thread "<<tdL.idThread<<endl;
        perror ("[Thread]Eroare la write() catre client.\n");
    }
    else
        cout<<"Message successfully sent to thread "<<tdL.idThread<<endl;
}
static void *treat(void* arg) //function executed by each client
{
    struct thData tdL;
    tdL= *((struct thData*)arg);
    cout<<"Inside thread "+tdL.idThread<<". Awaiting message..\n";
    fflush (stdout);
    pthread_detach(pthread_self());
    answer((struct thData*)arg);
    /* am terminat cu acest client, inchidem conexiunea */
    close ((intptr_t)arg);
    return(NULL);
}

ServerNetwork::ServerNetwork() {}

void ServerNetwork::Initialize(void *data, unsigned short port) {
    server=(Server *)data;
    portNum=port;
    if ((client_sd=socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error establishing connection." << endl;
        exit(1);
    }
    int on=1;
    setsockopt(client_sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    bzero(&server_addr,sizeof(server_addr));
    bzero(&from,sizeof(from));
    cout << "Server socket has been created" << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY); //accept any incoming messages
    server_addr.sin_port = htons(portNum);

    //binding socket = assigning an address to a socket
    if (bind(client_sd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) ==1) {
        cout << "Error binding socket" << endl;
        exit(1);
    }
}
void ServerNetwork::Listen() {
    if (listen(client_sd, 2) == -1) {
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
        if((client=accept(client_sd,(struct sockaddr *) &from,&length))<0){
            perror("Error on accepting client connection.");
            continue;
        }
        cout<<"Client #"<<clientCount+1<<" connected\n";
        td=(struct thData*)malloc(sizeof(struct thData));
        td->idThread=clientCount++;
        td->cl=client;
        pthread_create(&threads[clientCount],NULL,&treat,td);


    }
}

