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


int fileSize(char* request){
    int filelength;

    //open file
    FILE* file = fopen(request, "r");

    //find length
    if(!file){
        printf("File not found\n");
        return -1;
    }

    fseek(file, 0L, SEEK_END);
    filelength = ftell(file);
    fseek(file, 0L, SEEK_SET);
    printf("File Size: %d\n", filelength);
    fclose(file);
    return filelength;
}


int response_builder(char* request, char* headers, char* Contents, int filelen){
    //initialize variables
    int output_len;
    
    //retrieve file
    FILE* file = fopen(request, "r");

    //generate response string
    if(!file){
        sprintf(headers,"HTTP/1.1 404 Not Found\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: 0\r\n\r\n");
        return -1;
    }
    else{
        printf("%s successfully received\n", request);
        
        //read values into buff
        char c;
        int i = 0;
        while((c = fgetc(file)) != EOF){
            Contents[i] = c;
            i++;
        }
        Contents[i] = '\0';

        sprintf(headers,"HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: %d\r\n\r\n", filelen);
        
        output_len = (filelen + 1) + (strlen(headers));
    }
    fclose(file);
    return output_len;
}

int parseRequest(char* buffer, char* method, char* request, char* version){
    //parse strings
    char *PathStart = strchr(buffer, ' ') + 1;
    char *PathEnd = strchr(PathStart, ' ');
    char *httpEnd = strchr(PathEnd + 1, '\r');

    strncpy(method, buffer, PathStart - buffer - 1);
    strncpy(request, PathStart, PathEnd - PathStart);
    strncpy(version, PathEnd+1, httpEnd - PathEnd-1);

    method[strlen(method)] = '\0';
    version[strlen(version)] = '\0';

    //format request
    memmove(request, request + 1 , strlen(request));
    request[strlen(request)] = '\0';
    printf("Requested File: %s\n", request);

    if(strcmp(method, "GET") != 0){
        printf("Failed Parsing: Method\n");
        return -1;
    }
    if(strcmp(request, "response.html") != 0){
        printf("Failed Parsing: Request\n");
        return -1;
    }
    if(strcmp(version, "HTTP/1.1") != 0){
        printf("Failed Parsing: Version\n");
        return -1;
    }
    
    return strlen(buffer);
}