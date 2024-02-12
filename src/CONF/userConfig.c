#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PATH_SIZE 2000

//test


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

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "PATH = %[^\n]", path) == 1) {
            fclose(file);
            char *defPath;
            strcpy(defPath, path);
            free(path);
            //return strdup(path);  // strdup allocates memory and copies the string
            return defPath;
        }
    }

    fclose(file);

    perror("Errore nell'identificazione del PATH");
    free(path);  // Free the allocated memory
    return NULL;
}



/*



gcc -o server main_server.c handle_request.c serve_file.c start_server.c /home/ubuntu/SEMIFEF/kttp_server/LOG/log.c /home/ubuntu/SEMIFEF/kttp_server/CONF/userConfig.c






int main() {
    FILE *file = fopen("userconf.ini", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    const char *PATTONE = "userconf.ini";
    int port = get_PORT(file);
    char *path = get_PATH(PATTONE);

    if (port != -1) {
        printf("PORT: %d\n", port);
    } else {
        printf("PORT entry not found\n");
    }

    if (path != NULL) {
        printf("PATH: %s\n", path);
    } else {
        printf("PATH entry not found\n");
    }


    fclose(file);

    return 0;
}

*/
