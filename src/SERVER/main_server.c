#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <pthread.h>

#include "/usr/lib/kttp_server_src/LOG/log.h"
#include "/usr/lib/kttp_server_src/CONF/userConfig.h"
#include "constants.h"
#include "handle_request.h"
#include "serve_file.h"
#include "start_server.h"

typedef struct {
    int clientSocket;
    char* filePath;
} ThreadArgs;

void print_client_info(struct sockaddr_in client_addr);
void *handle_connection(void *arg);

int main() {
    //reading user configurations and storing data into variables
    const char *filename = "/etc/kttp_server/CONFs/userconf.ini";
    FILE *CONF_FILE = fopen(filename, "r");
    if (CONF_FILE == NULL) {
        perror("Error opening file");
        return 1; // Exit the program with an error code
    }
    int userConfPORT = get_PORT(CONF_FILE);
    char *userConfPath = get_PATH(filename);
    fclose(CONF_FILE);

    printf("%d is the port chosen by the user.\n", userConfPORT);
    printf("%s is the path where the user has chosen to store files.\n", userConfPath);

    //initializing server with user configs
    printf("\nKTTP server initialized and tarted listening on port %d \n", userConfPORT);
    int serverSocket = startServer(userConfPORT);

    while (1) {
        struct sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);

        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (clientSocket < 0) {
            perror("Error accepting client connection");
            continue;
        }

        ThreadArgs *args = (ThreadArgs *)malloc(sizeof(ThreadArgs));
        if (args == NULL) {
            perror("Error allocating memory for thread arguments");
            close(clientSocket);
            continue;
        }

        args->clientSocket = clientSocket;
        args->filePath = userConfPath;

        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_connection, args) != 0) {
            perror("Error creating thread");
            close(clientSocket);
            free(args);
            continue;
        }

        //pthread_detach(thread); EDITED
        print_client_info(clientAddress);

         if (pthread_join(thread, NULL) != 0) {
        perror("Error joining thread");
    }
    }

    close(serverSocket);
    free(userConfPath); // Free the memory allocated in get_PATH

    return 0;
}

void print_client_info(struct sockaddr_in client_addr) {
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    printf("\n\n\nConnected client IP: %s\n", client_ip);
    printf("Connected client port: %d\n", ntohs(client_addr.sin_port));
    writeLog(client_ip);
}

void *handle_connection(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    int clientSocket = args->clientSocket;
    char *filePath = args->filePath;
    free(args);

    handleRequest(clientSocket, filePath);

    close(clientSocket);

    pthread_exit(NULL);
}
