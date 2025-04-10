#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int sockfd, addrsize, recv_len;
    struct sockaddr_in addr;
    char buf[4096];

    if(argc != 3){
        perror("Insufficient Arguments");
        exit(1);
    }

    printf("Input String to send to buffer: ");
    scanf("%[^\n]", buf);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        perror ("Error: Creating Socket Failed");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(sockfd, (const struct sockaddr_in*) &addr, sizeof(addr))== -1){
        perror("Error: Failure Connecting to Socket");
        exit(1);
    }

    if(send(sockfd, buf, sizeof(buf), 0) < 0){
        perror("Error: Send Failure");
        exit(1);
    }


    if( (recv_len = recv(sockfd, buf, sizeof(buf) - 1, 0)) < 0){
        perror("Error: Receive Failure");
        exit(1);
    }

    buf[recv_len] = '\0';
    printf("Got %d bytes from server: %s\n", recv_len, buf);

    printf(" Client is done\n");
    close(sockfd);
}