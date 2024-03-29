
/*
*
*
*
* server_file.c - Copyright by gabricampaa, 2024
*
*
*
*
*/
	
	


#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "/usr/lib/kttp_server_src/CONF/userConfig.h"
#include "/usr/lib/kttp_server_src/LOG/log.h"


void serveFile(int clientSocket, const char *filePath) {
    FILE *file = fopen(filePath, "r");
    char *fileContent = NULL;
  if (strcmp(filePath, "/") == 0) {
        char redirectionResponse[] = "HTTP/1.1 302 Found\r\n"
                                     "Location: /index.html\r\n"
                                     "\r\n";
            write(clientSocket, redirectionResponse, sizeof(redirectionResponse) - 1);
            logStatus("Index handled succesfully!\n");

        return;
    }

    // File not found, serve 404 page
    if (file == NULL) {
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
            return;
        }
        logStatus("404 response handled\n");

        fseek(f404, 0, SEEK_END);
        long fileSize = ftell(f404);
        fseek(f404, 0, SEEK_SET);
        
        
        char *fileContent = (char *)malloc(fileSize + 1);  // +1 for null terminator
    

        fread(fileContent, 1, fileSize, f404);
        fileContent[fileSize] = '\0';  // Null-terminate the content
        fclose(f404);

        if (fileContent == NULL){
        perror("Error allocating memory for file content");
        fclose(f404);
        free(fileContent); 
        return;
        }

        char response404[MAX_RESPONSE_SIZE];
        snprintf(response404, sizeof(response404),
             "HTTP/1.1 404 Not Found\r\n" 
             "Content-Type: text/html\r\n"
             "Content-Length: %ld\r\n"
             "\r\n"
             "%s", fileSize, fileContent);

        write(clientSocket, response404, strlen(response404));
        fclose(f404); //added in v0.2
       //free(f404);
            free(fileContent);


    return;
    }


    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    fileContent = (char *)malloc(fileSize + 1);  // +1 for null terminator


    fread(fileContent, 1, fileSize, file);
    fileContent[fileSize] = '\0';  // Null-terminate the content
    fclose(file);
    if (fileContent == NULL) {
        perror("Error allocating memory for file content lol");
        fclose(file);
        return;
    }
    char response[MAX_RESPONSE_SIZE];
    snprintf(response, sizeof(response),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "Content-Length: %ld\r\n"
             "\r\n"
             "%s", fileSize, fileContent);

            write(clientSocket, response, strlen(response));

            free(fileContent);
            logStatus("File requested handled succesfully!\n");

}
