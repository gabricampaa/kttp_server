/*
*
*
*
* handle_request.c - Copyright by gabricampaa, 2024
*
*
*
*
*/
	

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

//le personalizzazioni
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

    printf("Received request: Method=%s, Path=%s\n\n", httpRequest.method, httpRequest.path);

    // Constructing the full file path by combining the base directory and the requested path
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s%s", baseDir, httpRequest.path);
    writeLog(requestBuffer);

    //std response,aka landing on index.html
    if(strcmp(httpRequest.path, "/")==0){
          const char *str1 = "/var/kttp_server_files/html_docs/index.html";
          serveFile(clientSocket, str1);
        logStatus("Index handled succesfully!\n");

        }   
        
         // Handle the request and send the response
    if (strcmp(httpRequest.method, "GET") == 0) {
        serveFile(clientSocket, filePath);
    } else if(strcmp(httpRequest.method, "HEAD") == 0){

        char headResponse[] = "HTTP/1.1 200 OK\r\n\r\nContent-Type:html\r\n\r\nSup m8!";
        write(clientSocket, headResponse, sizeof(headResponse) - 1);
        logStatus("Request handled succesfully!\n");
        
    }else {
        char notImplementedResponse[] = "HTTP/1.1 501 Not Implemented\r\n\r\nMethod not implemented.";
        write(clientSocket, notImplementedResponse, sizeof(notImplementedResponse) - 1);
    }
}
