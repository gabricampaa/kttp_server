#include <limits.h>  // Aggiunto per PATH_MAX
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>  // Aggiunto per la funzione realpath
#include <limits.h>  // Aggiunto per la costante PATH_MAX


#include <fcntl.h>
#include <time.h>
#include <signal.h>

#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>


#include "/usr/lib/kttp_server_src/LOG/log.h"
#include "/usr/lib/kttp_server_src/CONF/userConfig.h"
#include "constants.h"
#include "handle_request.h"
#include "serve_file.h"
#include "start_server.h"



char filePath[MAX_PATH_SIZE];




void print_client_info(struct sockaddr_in client_addr);

int main() {


    //file di configurazione, lettura dei dati meessi dall'utente 
    const char* filename = "/etc/kttp_server/CONFs/userconf.ini";

    FILE *CONF_FILE = fopen("/etc/kttp_server/CONFs/userconf.ini", "r");
     if (CONF_FILE == NULL) {
        perror("Error opening file");
        return 1;  // Exit the program with an error code
    }
    int userConfPORT = get_PORT(CONF_FILE);
    char* userConfPath = get_PATH(filename);

    fclose(CONF_FILE);
    printf("%d essere porta scelta utente\n", userConfPORT);
    printf("%s I file sono qui, invece\n", userConfPath);


//inizializazione server con dati utente
    printf("\nVersione 1.0 del server inizializzata. Started listening on port %d \n", userConfPORT);
    int serverSocket = startServer(userConfPORT);
logStatus("Server initialized.\n");

    while (1) {
        struct sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);

        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (clientSocket < 0) {
            perror("Error accepting client connection");
            continue;
        }

        pid_t childPid = fork();

        if (childPid == 0) {
            // Child process
            close(serverSocket); // Close the listening socket in the child process
            handleRequest(clientSocket, userConfPath);
            char client_msg[4096] = "";
		    int client_socket = accept(clientSocket, NULL, NULL);

		    read(client_socket, client_msg, 4095);
		    printf("%s\n", client_msg);

            close(clientSocket);
            exit(EXIT_SUCCESS);
        } else if (childPid > 0) {
            // Parent process
            close(clientSocket);
        } else {
            perror("Error forking process");
        }
        print_client_info(clientAddress);
    }

    close(serverSocket);
    free(userConfPath);  // Free the memory allocated in get_PATH


    return 0;
}



void print_client_info(struct sockaddr_in client_addr) {
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    printf("\n\n\nConnected client IP: %s\n", client_ip);
    printf("Connected client port: %d\n", ntohs(client_addr.sin_port));
    writeLog(client_ip);
}

//kttp serveer puoi eliminarlo
