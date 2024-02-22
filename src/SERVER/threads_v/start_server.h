typedef struct {
    char method[10];
    char path[256];
} HttpRequest;

int startServer(int port);