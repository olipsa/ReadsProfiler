#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <ifaddrs.h>

using namespace std;
string getIPAddress(){
    string ipAddress="Unable to get IP Address";
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while(temp_addr != NULL) {
            if(temp_addr->ifa_addr->sa_family == AF_INET) {
                // Check if interface is en0 which is the wifi connection on the iPhone
                if(strcmp(temp_addr->ifa_name, "en0")){
                    ipAddress=inet_ntoa(((struct sockaddr_in*)temp_addr->ifa_addr)->sin_addr);
                }
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
    // Free memory
    freeifaddrs(interfaces);
    return ipAddress;
}
int main(){
    int client; //file descriptor containing the value returned by the socket system call
    int portNum=8088;
    const char* ip="127.0.0.1";

    client=socket(AF_INET,SOCK_STREAM,0);
    if(client<0){
        cout<<"Error with socket client"<<endl;
        exit(1);
    }

    cout<<"Socket client has been created"<<endl;
    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr(ip);
    server_addr.sin_port=htons(portNum);

    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        cout<<"Connection to the server failed\n";
        exit(0);
    }
    int bufsize=1024;
    char buffer[bufsize];
    cout<<"Connected to the server"<<endl;

    cout<<"Enter your message: ";
    fflush(stdout);
    cin>>buffer;
    if(write(client,buffer,bufsize)==-1){
        cout<<"Error when sending message\n";
        exit(0);
    }
    cout<<"Message sent successfully\n";
    if(read(client, buffer,bufsize)==-1){
        cout<<"Error when receiving message\n";
        exit(0);
    }
    cout<<"Received message is: \""<<buffer<<"\"\n";
    cout<<"Your ip address is: "<<getIPAddress()<<endl;
    close(client);



}

