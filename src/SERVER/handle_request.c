#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "start_server.h"
#include "constants.h"
#include "serve_file.h"
#include "/usr/lib/kttp_server_src/LOG/log.h"


void handleRequest(int clientSocket, const char* baseDir) {

    printf("=========================================\n");
    char requestBuffer[MAX_REQUEST_SIZE];
    ssize_t bytesRead = read(clientSocket, requestBuffer, sizeof(requestBuffer) - 1);
    if (bytesRead < 0) {
        perror("Error reading from socket");
        return;
    }
    requestBuffer[bytesRead] = '\0';
    printf("\n%s\n", requestBuffer);
    HttpRequest httpRequest;
    sscanf(requestBuffer, "%s %s", httpRequest.method, httpRequest.path);
    writeLog(requestBuffer);

    printf("Received request: Method=%s, Path=%s\n\n", httpRequest.method, httpRequest.path);

    // Constructing the full file path by combining the base directory and the requested path
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s%s", baseDir, httpRequest.path);

    //std response,aka landing on index.html
    if(strcmp(httpRequest.path, "/")==0){
          const char *str1 = "/var/kttp_server_files/html_docs/index.html";
          serveFile(clientSocket, str1);
        }   
        
         // Handle the request and send the response
    if (strcmp(httpRequest.method, "GET") == 0) {
        serveFile(clientSocket, filePath);
    } else {
        char notImplementedResponse[] = "HTTP/1.1 501 Not Implemented\r\n\r\nMethod not implemented.";
        write(clientSocket, notImplementedResponse, sizeof(notImplementedResponse) - 1);
    }
}
