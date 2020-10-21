#include "csapp.h"
#include "file_server.h"

int main(int argc, char** argv) {

    // Local variables for application support
    unsigned int secretKey;
    int listenfd;
    int connfd;
    int port;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;

    // Make sure the server is invoked with all information it needs
    if (argc != 3) {
        fprintf(stderr, "usage: %s port secretKey\n", argv[0]);
        exit(0);
    }

    // Store port and secretKey information for the application
    port = atoi(argv[1]);
    secretKey = (unsigned int)atoi(argv[2]);

    if ((port == 0) && (secretKey == 0))
    {
        printf("Port and secretKey cannot both be 0\n");
        exit(EXIT_FAILURE);
    }
    else if (secretKey == 0)
    {
        printf("Secret key cannot equal 0\n");
        exit(EXIT_FAILURE);
    }
    else if (port == 0)
    {
        printf("Port cannot equal 0\n");
        exit(EXIT_FAILURE);
    }

    // Start listening on the specified port
    listenfd = Open_listenfd(port);

    while (1) {

        // Start listening for connections
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA*)& clientaddr, &clientlen);

        // Handle the request
        handle_request(connfd, &secretKey);

        // Close the connection
        Close(connfd);

    }

    exit(0);
}
