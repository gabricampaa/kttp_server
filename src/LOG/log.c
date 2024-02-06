#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "/usr/lib/kttp_server_src/SERVER/start_server.h"
#include "/usr/lib/kttp_server_src/SERVER/constants.h"
#include "/usr/lib/kttp_server_src/SERVER/serve_file.h"


char *dateTime(void) {
    time_t currentTime;
    time(&currentTime);

    // Convert the current time to a string representation
    char *timeString = ctime(&currentTime);

    // Allocate memory for a new string and copy the timeString
    char *result = malloc(strlen(timeString) + 1);
    
    strcpy(result, timeString);

    return result;
}
void writeLog(const char *clientIP) {
    FILE *fptr;
    char *dataEora = dateTime();

    // Specify the folder and file path
    char logDir[256];
    snprintf(logDir, sizeof(logDir), "/var/log/kttp_log");

    // Check if the directory exists, if not, create it
    struct stat st = {0};
    if (stat(logDir, &st) == -1) {
        if (mkdir(logDir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == -1) {
            perror("Error creating log directory");
            free(dataEora);  // Free memory allocated in dateTime
            return;
        }
    }

    char filePath[256];
    snprintf(filePath, sizeof(filePath), "/var/log/kttp_log/connections.log");

    // Open the file in append mode
    if ((fptr = fopen(filePath, "a")) == NULL) {
        perror("Error opening log file");
        free(dataEora);  // Free memory allocated in dateTime
        return;
    }

    // Write data to the log file
    fprintf(fptr, "Client IP: %s\n", clientIP);
    fprintf(fptr, "Timestamp: %s", dataEora);
    fprintf(fptr, "\n\n=====END OF CONNECTION=====\n\n\n");

    fclose(fptr);
    free(dataEora);  // Free memory allocated in dateTime
}
