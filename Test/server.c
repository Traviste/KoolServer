#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

void reverseString(char str[]) {
    int length = strlen(str);
    int start = 0;
    int end = length - 1;
    char output[length];

    // Reversing the string
    while (start < length) {
        output[start] = str[end];
        start++;
        end--;
    }

    // Null-terminate the reversed string
    output[length] = '\0';

    // Copying the reversed string back to the original string
    strcpy(str, output);
}

int main(int argc, char * argv[]){
    int sockfd, clientsock, mlen;
    struct sockaddr_in servaddr;
    char ch, buf[4096];

    if (argc != 2){
        perror("Invalid Number of Arguments\n");
        exit(-1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        perror("Opening Socket Error\n");
        exit(-1);
    }

    servaddr.sin_family=AF_INET;
    servaddr.sin_port = htons(atoi(argv[1]));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, &servaddr, sizeof(struct sockaddr_in)) != 0)
    {
        perror("Socket binding Failed\n");
        exit(-1);
    }

    if (listen(sockfd, 1) != 0){
        perror("Error: On listen\n");
        exit(-1);
    }
    //printf("connection made with client %s", inet_ntoa(addr. sin_addr));
    int len = sizeof(servaddr);
    clientsock = accept(sockfd, &servaddr, &len);
    if(clientsock == -1){
        perror("Error: Client Accept Failed");
        exit(-1);
    }

    mlen = recv(clientsock, buf, 4090, 0);
    buf[mlen] = '\0';
    if(mlen == -1){
        perror("Error: Failure receive message");
        exit(-1);
    }
    else{
        printf("Normal String Received: %s\n", buf);
    }

    //reverse received string
    reverseString(buf);

    printf("Got %d bytes: %s\n", strlen(buf), buf);
    send(clientsock, buf, strlen(buf), 0);

    printf("Server: all messages read.....connections closing\n");
    close(clientsock);
    close(sockfd);

    return 0;
}