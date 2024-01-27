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

void writeLog(char *clientIP) {
FILE *fptr;
    char *dataEora = dateTime();

    // Specify the folder and file path
    char logDir[256];
    snprintf(logDir, sizeof(logDir), "/var/log/kttp_log");

    // Check if the directory exists, if not, create it
    struct stat st = {0};
    if (stat(logDir, &st) == -1) {
        mkdir(logDir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    }

    char filePath[256];
    snprintf(filePath, sizeof(filePath), "/var/log/kttp_log/connections.log");

    // Open the file in writing mode
    if ((fptr = fopen(filePath, "a")) == NULL) {
        perror("Error opening file");
        // Handle the error as needed
        free(dataEora);  // Don't forget to free the memory allocated in dateTime
        return;
    }

    // Write some text to the file
    fprintf(fptr, "%s", dataEora);
    fprintf(fptr, " Connection from %s\n\n", clientIP);

    fclose(fptr);
    // Don't forget to free the memory allocated in dateTime
    free(dataEora);
}
