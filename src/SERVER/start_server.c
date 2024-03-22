/*
*
*
*
* start_server.c - Copyright by gabricampaa, 2024
*
*
*
*
*/
	

#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"




//struttra della richiesta http
typedef struct {
    char method[10];
    char path[256];
} HttpRequest;


int startServer(int port) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error creating server socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error binding server socket");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 5) < 0) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }


    return serverSocket;
}

