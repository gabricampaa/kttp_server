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
    size_t len = strlen(timeString) + 1;
    char *result = malloc(len);

    if (result != NULL) {
        // Use strncpy to copy, limiting to the available space
        strncpy(result, timeString, len);
        // Ensure null-termination
        result[len - 1] = '\0';
    }

    return result;
}

void writeLog(char *clientIP) {
    FILE *fptr;
    char *dataEora = dateTime();

    // Check if dateTime returned a valid pointer
    if (dataEora == NULL) {
        // Handle the error as needed
        return;
    }

    // Specify the folder and file path
    char filePath[256];
    mkdir("log", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    snprintf(filePath, sizeof(filePath), "log/connections.log");

    // Open the file in writing mode
    if ((fptr = fopen(filePath, "a")) == NULL) {
        perror("Error opening file");
        // Handle the error as needed
        free(dataEora);  // Don't forget to free the memory allocated in dateTime
        return;
    }

    // Write the log entry using snprintf to prevent buffer overflow
    snprintf(filePath, sizeof(filePath), "%s Connection from %s\n\n", dataEora, clientIP);
    fprintf(fptr, "%s", filePath);

    fclose(fptr);
    // Don't forget to free the memory allocated in dateTime
    free(dataEora);
}
