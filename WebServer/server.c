#include <sys/socket.h>
#include <stdio.h>
#include <sys/poll.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"

int main (int argc, char* argv[]){
    int sockfd, clientsock, mlen;
    int opt = 1;
    char buffer [BUFFER_LEN] = {0};
    struct sockaddr_in servaddress, clientaddress;
    socklen_t addresslen = sizeof(servaddress);
    socklen_t clientlen = sizeof(clientaddress);

    //create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("Error: Failed to Create Socket");
        exit(-1);
    }

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(-1);
    }

    memset(&servaddress, 0, sizeof(servaddress));
    servaddress.sin_family = AF_INET;
    servaddress.sin_port = htons(SERVER_PORT);
    servaddress.sin_addr.s_addr = INADDR_ANY;


    //bind socket to the ip address
    if(bind(sockfd, (struct sockaddr*) &servaddress, addresslen) <0){
        perror("Error: binding to socket");
        exit(-1);
    }

    //listen for the connection
    if((listen(sockfd, BACKLOG)) < 0){
        perror("Error: Failed to listen");
        exit(-1);
    }

    while(1){
        //accept connection
        clientsock = accept(sockfd, (struct sockaddr*) &clientaddress, (socklen_t*) &clientlen);
        if(clientsock < 0){
            perror("Error: Failed to accept communication request");
            exit(-1);
        }

        //receive packet http://localhost:8080/hello.html
        mlen = recv(clientsock, buffer, BUFFER_LEN - 1, 0);
        printf("%lu bytes received\n", strlen(buffer));
        printf("Buffer Msg Received: %s", buffer);

        if(mlen < 0){
            perror("Failed to receive packet");
            close(clientsock);
            continue;
        }
        buffer[mlen] = '\0';
        
        char* method=  (char*) malloc(sizeof(char) * 50);
        char* request= (char*) malloc(sizeof(char) * 50);
        char* version= (char*) malloc(sizeof(char) * 50);

        if(network(buffer, method, request, version, clientsock ) < 0){
            perror("Failed Parsing\n");
            close(clientsock);
            continue;
        }

        close(clientsock);

        //free dynamically allocated memory
        free(method);
        free(request);
        free(version);
    }

    //close client and server sockets
    close(sockfd);

}

int network(char* input, char* method, char* request, char* version, int clientsock){
    //parse request
    int input_length = parseRequest(input, method, request, version);

    if(input_length < 0){
        printf("Method Parsed: %s\n", method);
        printf("Request Parsed: %s\n", request);
        printf("Version Parsed: %s\n", version);
        return -1;
    }

    //retrieve file metadata
    int filelen;
    filelen = fileSize(request);  //create char array for the file buffer

    char* Contents = (char*)malloc(sizeof(char)* filelen);
    char* headers = (char*)malloc(sizeof(char) * (filelen + BUFFER_LEN));
    headers[0] = '\0';
    int output_length = response_builder(request, headers, Contents,  filelen);

    if(output_length < 0){
        printf("Failed Response Build\n");
        send(clientsock, headers, strlen(headers), 0 ); //send 404 response
        free(Contents);
        free(headers);
        return -1;
    }

    //generate output buffer, and send over socket then ... free(output);
    char * output = (char*) malloc(sizeof(char) * output_length);
    memcpy(output, headers, strlen(headers));
    memcpy(output+ (strlen(headers)), Contents, filelen);

    if((send(clientsock, output, output_length, 0)) < 0){
        perror("Failed Sending Data");
        exit(-1);
    }

    free(Contents);
    free(headers);
    free(output);
    return 1;
}