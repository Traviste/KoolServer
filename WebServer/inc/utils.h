#ifndef UTILS_H
#define UTILS_H

#define SERVER_PORT 8080
#define BUFFER_LEN 100000
#define BACKLOG 10

int fileSize(char* request);
int response_builder(char* request, char* headers, char* Contents, int filelen);
int parseRequest(char* buffer, char* method, char* request, char* version);
typedef struct http_params{
	char method[50];
	char request[50];
	char version[50];
} http_params;

int network(char* input, int clientsock, http_params* http);

#endif //UTILS_H