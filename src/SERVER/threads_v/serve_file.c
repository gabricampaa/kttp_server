#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "/usr/lib/kttp_server_src/CONF/userConfig.h"
#include "/usr/lib/kttp_server_src/LOG/log.h"

void serveFile(int clientSocket, const char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        // File not found, serve 404 page
        char full404Path[MAX_PATH_SIZE];
        const char* filename = "/etc/kttp_server/CONFs/userconf.ini";
        char* userConfPath = get_PATH(filename);
        snprintf(full404Path, sizeof(full404Path), "%s/404.html", userConfPath);

        FILE *f404 = fopen(full404Path, "r");
        if (f404 == NULL) {
            // 404.html not found, serve a generic 404 response
            char response404[] = "HTTP/1.1 404 Not Found\r\n"
                                 "Content-Type: text/html\r\n"
                                 "\r\n"
                                 "<html><body><h1>404 Not Found</h1></body></html>";
            write(clientSocket, response404, sizeof(response404) - 1);

            perror("Error opening 404.html");
            free(userConfPath);
            return;
        }

        fseek(f404, 0, SEEK_END);
        long fileSize = ftell(f404);
        fseek(f404, 0, SEEK_SET);

        char *fileContent404 = (char *)malloc(fileSize + 1);
        if (fileContent404 == NULL) {
            perror("Error allocating memory for 404 file content");
            fclose(f404);
            free(userConfPath);
            return;
        }

        fread(fileContent404, 1, fileSize, f404);
        fileContent404[fileSize] = '\0';  // Null-terminate the content
        fclose(f404);

        char response404[MAX_RESPONSE_SIZE];
        snprintf(response404, sizeof(response404),
                 "HTTP/1.1 404 Not Found\r\n" 
                 "Content-Type: text/html\r\n"
                 "Content-Length: %ld\r\n"
                 "\r\n"
                 "%s", fileSize, fileContent404);

        write(clientSocket, response404, strlen(response404));

        free(fileContent404);
        free(userConfPath);
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *fileContent = (char *)malloc(fileSize + 1);
    if (fileContent == NULL) {
        perror("Error allocating memory for file content");
        fclose(file);
        return;
    }

    fread(fileContent, 1, fileSize, file);
    fileContent[fileSize] = '\0';  // Null-terminate the content
    fclose(file);

    char response[MAX_RESPONSE_SIZE];
    snprintf(response, sizeof(response),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "Content-Length: %ld\r\n"
             "\r\n"
             "%s", fileSize, fileContent);

    write(clientSocket, response, strlen(response));
    pid_t piddo = getpid();
    printf("\nID PID: %d\n", piddo);
    free(fileContent);
}
