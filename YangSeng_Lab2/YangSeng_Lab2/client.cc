/*
*   Seng Xai Yang
*   CS 452
*   client.cc
*   Client reads in a file encrypts it
*   and sends it to the server.
*/
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h> // this is used for IP Socket data types */ 
#include <netdb.h>
#include <arpa/inet.h> // this is used for sockaddr_in and inet_addr(
#include<sys/types.h>  
#include<sys/socket.h> // this is used for socket(), connect(), send(), and recv()
#include <unistd.h>

using namespace std;

#define SA struct sockaddr
#define BYTES_DBG //for debugging when bytes are sent

char* encrypt(char* originalString, char* encKey);

int main() {

    int sockfd;
    int port;
    int pktSize;
    int conn;
    char* encKey = (char*)malloc(sizeof(char));
    char* ipAddress = (char*)malloc(sizeof(char));
    char* fileName = (char*)malloc(sizeof(char));

    struct sockaddr_in sockaddr;

    // gets ip address
    cout << endl;
    cout << "Connect to IP Address: ";
    cin >> ipAddress;
    
    //gets the port
    cout << "Port #: ";
    cin >> port;

    //gets file name
    cout << "File to be sent: ";
    cin >> fileName;

    //gets packet size
    cout << "Pkt size: ";
    cin >> pktSize;

    if(pktSize > 64)
    {
        cout << "Packet size must be less than 64\n";
        return 0;
    }
    //gets encryption key
    cout << "Enter encryption key: ";
    cin >> encKey;
    cout << endl;

    //opens the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1)
    {
        cout << "Creating socket failed" << endl;
        return 0;
    }
    

    bzero(&sockaddr, sizeof(sockaddr)); 

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr.s_addr = inet_addr(ipAddress);
    //sockaddr.sin_addr.s_addr = inet_addr("10.34.40.33"); // phoenix1
    //sockaddr.sin_addr.s_addr = inet_addr("10.35.195.250"); //phoenix2
    //sockaddr.sin_addr.s_addr = inet_addr("10.35.195.236");//phoenix3 10.35.195.236
    
    //convert kb to bytes
    //1024 bytes = 1 kb
    int bytes = pktSize * 1024;
   
    if (connect(sockfd, (SA*)&sockaddr, sizeof(sockaddr)) != 0) { 
        cout << "Connection to server failed" << endl;
        exit(0); 
    } 
    
    //opening file
    ifstream fin(fileName, ios::in); // for testing can replace with '/tmp/2M'

    //checking if valid file
    if(fin.fail())
    {
        cout << "Error opening file" << endl;
        exit(0);
    }

    char* buffer = (char*) malloc (sizeof(char)*bytes);
    char* buffer2 = (char*) malloc (sizeof(char)*bytes);
    int counter = 0;
    while(1) {
        //read file into buffer
        fin.read(buffer, bytes);
        std::streamsize s = fin.gcount(); //char count

        if(s == 0){break;}

        int *p;

        //encrypt buffer
        buffer2 = encrypt(buffer, encKey); //encrypt

        while(s > 0){ //char count > 0 then continue writing 
            
            int bytes_written = write(sockfd, buffer2, s); //send encrypted stuff over
            
#ifdef BYTES_DBG
            if(counter < 10)
            {
                cout << "Sent encrypted packet# " << counter << " Bytes written: " << bytes_written << endl;
            }
#endif

            if(bytes_written <= 0){
                //cout << "ERROR";
                break;
            }

            counter++;
            s -= bytes_written;
            p += bytes_written;
            bzero(buffer2, sizeof(buffer2));
        }
        bzero(buffer, sizeof(buffer));

        ///do with buffer
    }
   

    cout << "Send Success!" << endl;

    close(sockfd);
    free(ipAddress);
    free(buffer);
    free(buffer2);
    free(encKey);
    free(fileName);
  return 0;
}

char* encrypt(char* originalString, char* encKey){
    //char key[] = "fox";
    int len = strlen(originalString);
    char * output = new char[len + 1];
    strcpy(output, originalString);
    for(int i = 0; i < len; i++){
        
        output[i] = originalString[i] ^ encKey[i % (sizeof(encKey) / sizeof(char))];
        
    }

    return output;
}


