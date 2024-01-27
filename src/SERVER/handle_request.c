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

    // Construct the full file path by combining the base directory and the requested path
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s%s", baseDir, httpRequest.path);

//std response,aka landing on index.html
    if(strcmp(httpRequest.path, "/")==0){
        //serveFile(clientSocket, "/home/ubuntu/SEMIFEF/kttp_server/html/index.html");
       const char *str1 = "/var/kttp_server_files/html_docs/index.html";
        size_t totalLength = strlen(baseDir) + strlen(str1) + 1; // +1 for the null terminator
        char *concatenated = (char *)malloc(totalLength);
        
    if (concatenated != NULL) {
    // Copy the base directory
    strncpy(concatenated, baseDir, totalLength);
    concatenated[totalLength - 1] = '\0'; // Ensure null-termination

    // Concatenate the file name using strncat
    strncat(concatenated, str1, totalLength - strlen(baseDir) - 1);

    // Use the concatenated string as needed

    // Don't forget to free the allocated memory
    free(concatenated);
    } else {
    // Handle memory allocation error
    perror("Error allocating memory for concatenated path");
    }

        // Serve the file using the concatenated path
        serveFile(clientSocket, concatenated);

        // Don't forget to free the allocated memory
        free(concatenated);

        }   
        
         // Handle the request and send the response
    if (strcmp(httpRequest.method, "GET") == 0) {
        serveFile(clientSocket, filePath);
    } else {
        char notImplementedResponse[] = "HTTP/1.1 501 Not Implemented\r\n\r\nMethod not implemented.";
        write(clientSocket, notImplementedResponse, sizeof(notImplementedResponse) - 1);
    }
}
