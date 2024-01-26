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
#include <sys/wait.h>
#include <pthread.h> // Include for threads
#include <netdb.h>
#include "/usr/lib/kttp_server_src/LOG/log.h"
#include "/usr/lib/kttp_server_src/CONF/userConfig.h"
#include "constants.h"
#include "handle_request.h"
#include "serve_file.h"
#include "start_server.h"

char filePath[MAX_PATH_SIZE];

void *handle_client(void *arg);
void print_client_info(struct sockaddr_in client_addr);

int main() {
    const char *filename = "/etc/kttp_server/CONFs/userconf.ini";
    FILE *CONF_FILE = fopen("/etc/kttp_server/CONFs/userconf.ini", "r");

    if (CONF_FILE == NULL) {
        perror("Error opening file");
        return 1; // Exit the program with an error code
    }

    int userConfPORT = get_PORT(CONF_FILE);
    char *userConfPath = get_PATH(filename);

    fclose(CONF_FILE);
    printf("%d essere porta scelta utente\n", userConfPORT);
    printf("%s I file sono qui, invece\n", userConfPath);

    printf("\nVersione 1.0 del server inizializzata. Started listening on port %d \n", userConfPORT);
    int serverSocket = startServer(userConfPORT);

    while (1) {
        struct sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);

        int *clientSocket = (int *)malloc(sizeof(int));
        *clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);

        if (*clientSocket < 0) {
            perror("Error accepting client connection");
            free(clientSocket);
            continue;
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, clientSocket) != 0) {
            perror("Error creating thread");
            free(clientSocket);
            continue;
        }

        pthread_detach(tid); // Detach the thread to allow it to clean up after termination
        print_client_info(clientAddress);
    }

    close(serverSocket);
    free(userConfPath); // Free the memory allocated in get_PATH

    return 0;
}

void *handle_client(void *arg) {
    int clientSocket = *((int *)arg);
    free(arg); // Free the memory allocated for the argument

    handleRequest(clientSocket, userConfPath);
    close(clientSocket);
    return NULL;
}

void print_client_info(struct sockaddr_in client_addr) {
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    printf("\n\n\nConnected client IP: %s\n", client_ip);
    printf("Connected client port: %d\n", ntohs(client_addr.sin_port));
    writeLog(client_ip);
}
