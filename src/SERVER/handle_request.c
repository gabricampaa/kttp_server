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
    if (sscanf(requestBuffer, "%63s %255s", httpRequest.method, httpRequest.path) != 2) {
        fprintf(stderr, "Error parsing request\n");
        return;
    }

    writeLog(requestBuffer);

    printf("Received request: Method=%s, Path=%s\n\n", httpRequest.method, httpRequest.path);

    // Construct the full file path by combining the base directory and the requested path
    char filePath[512];  // Adjust the size based on your needs
    if (snprintf(filePath, sizeof(filePath), "%s%s", baseDir, httpRequest.path) >= sizeof(filePath)) {
        fprintf(stderr, "File path is too long\n");
        return;
    }

    // Standard response, i.e., landing on index.html
    if (strcmp(httpRequest.path, "/") == 0) {
        const char *str1 = "/var/kttp_server_files/html_docs/index.html";
        size_t totalLength = strlen(baseDir) + strlen(str1) + 1;  // +1 for the null terminator
        char *concatenated = (char *)malloc(totalLength);

        if (concatenated == NULL) {
            perror("Error allocating memory");
            return;
        }

        // Use strncpy to copy strings and avoid buffer overflow
        strncpy(concatenated, baseDir, totalLength);
        strncat(concatenated, str1, totalLength - strlen(concatenated) - 1);

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
