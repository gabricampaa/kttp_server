#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PATH_SIZE 2000

int get_PORT(FILE *file) {
    char line[256];
    int port = -1;  // Declare and initialize the 'port' variable

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == ';' || line[0] == '#') {
            // Skip comments
            continue;
        }

        // Look for the PORT entry
        if (sscanf(line, "PORT = %d", &port) == 1) {
            return port;
        }
    }

    // PORT entry not found
    return -1;
}





char* get_PATH(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Errore nell'apertura del file");
        return NULL;
    }

    char line[MAX_PATH_SIZE];
    char* path = (char*)malloc(MAX_PATH_SIZE); 
    if (path == NULL) {
        perror("Errore nell'allocazione della memoria");
        fclose(file);
        return NULL;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "PATH = %[^\n]", path) == 1) {
            fclose(file);
            char* result = strdup(path);  // strdup allocates memory and copies the string
            free(path);  // Free the allocated memory for path
            return result;
        }
    }

    fclose(file);

    perror("Errore nell'identificazione del PATH");
    free(path);  // Free the allocated memory for path
    return NULL;
}



/*
char* get_PATH(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Errore nell'apertura del file");
        return NULL;
    }

    char line[MAX_PATH_SIZE];
    char* path = (char*)malloc(MAX_PATH_SIZE); 
                                                                
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "PATH = %[^\n]", path) == 1) {
            fclose(file);
            return strdup(path);  // strdup allocates memory and copies the string
        }
    }

    fclose(file);

    perror("Errore nell'identificazione del PATH");
    free(path);  // Free the allocated memory
    return NULL;
}
*/


