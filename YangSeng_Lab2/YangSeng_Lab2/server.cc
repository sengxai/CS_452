/*
*   Seng Xai Yang
*   CS 452
*   server.cc
*   Server receives a file and decrypts it
*   and writes it to a directory/file.
*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <netinet/in.h> // this is used for IP Socket data types */
#include <netdb.h>
#include <arpa/inet.h> // this is used for sockaddr_in and inet_addr()
#include<sys/types.h> 
#include<sys/socket.h> // this is used for socket(), connect(), send(), and recv()
#include <unistd.h>

//#define PORT 9500
#define SA struct sockaddr
#define BYTES_DBG   //for debugging when bytes are received

using namespace std;

char* decrypt(char* originalString, char* encKey);

int main() {
    int bytes_read;
    int sockfd, bnd, lstn;
    int port;
    char* fileName = (char*)malloc(sizeof(char));
    char* encKey = (char*)malloc(sizeof(char));
    char* ipAddress = (char*)malloc(sizeof(char));

    socklen_t len;
    int client_sock;
    struct sockaddr_in sockaddr, client;
    char buffer[65536] = {0}; //adjust the size so nothing else gets read in...
    
    /*** Enter in IP Address ***/
    cout << endl;
    cout << "Connect to IP Address: ";
    cin >> ipAddress;
    
    /*** Enter in port number ***/
    cout << "Port #: ";
    cin >> port;

    /*** Enter in where file name
     * should be saved at
     ***/
    cout << "Save file to: ";
    cin >> fileName;
    cout << endl;

    /***Enter encryption key ***/
    cout << "Enter encryption key: ";
    cin >> encKey;
    cout << endl;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        cout << "Creating socket failed" << endl;
    }
    
    
    sockaddr.sin_family = AF_INET; 
    //sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    //sockaddr.sin_addr.s_addr = inet_addr("10.35.195.236");
    sockaddr.sin_addr.s_addr = inet_addr(ipAddress);
    sockaddr.sin_port = htons(9500); 

    // Binding to new socket
    bnd = bind(sockfd, (SA*)&sockaddr, sizeof(sockaddr));
    if (bnd < 0) { 
        cout << "Error binding to socket" << endl;
        exit(0); 
    } 
    

    //listens for a client connection
    lstn = listen(sockfd, 3); // listen up to 3 connections
    if(lstn < 0)
    {
        cout << "Error listening on socket" << endl;
    }
    
    len = sizeof(client);

    //accepts the client
    client_sock = accept(sockfd, (SA*)&client, &len); //accepts client

    if(client_sock < 0)
    {
        cout << "Error Accepting Client!!" << endl;
    }
    
    
    //reads what the client sent and prints it out

    
    int counter = 0;
    ofstream open(fileName, ios::out);
    char* buffer2 = (char*)malloc(sizeof(char)*65336);

    while(1){

        bytes_read = read(client_sock, buffer, sizeof(buffer)); //bytes read
            
#ifdef BYTES_DBG
        if(counter < 10)
        {
            cout << "Rec packet# " << counter << " Bytes received: " << bytes_read << endl;
        }
#endif

        buffer2 = decrypt(buffer, encKey); //decrypt
        if(bytes_read <= 0)
        {
            break;
        }
        //open << buffer;
        counter++;
        open.write(buffer, sizeof(buffer)); //write into file
        bzero(buffer, sizeof(buffer));
    }

    open.close();
    
    
    cout << "Receive Success!" << endl;
   
    free(ipAddress);
    free(encKey);
    free(fileName);
    close(sockfd);
    close(client_sock);
    //return 0;
}


char* decrypt(char* originalString, char* encKey){
    char key[] = "fox";
    int len = strlen(originalString);
    char * output = new char[len + 1];
    strcpy(output, originalString);
    for(int i = 0; i < len; i++){
        
        output[i] = originalString[i] ^ key[i % (sizeof(key) / sizeof(char))];
        
    }

    return output;
}
