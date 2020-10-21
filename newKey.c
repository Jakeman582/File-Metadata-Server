#include "csapp.h"
#include "file_server.h"

int main(int argc, char **argv) {

    // Local variables
        char *host;
        unsigned short port;
        unsigned int secret_key;
        unsigned int new_key;

        // Make sure there are enough arguments
        if(argc != 5)
        {
            fprintf(stdout, "usage: %s host port secret_key new_key\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        // Set networking and application data
        host = argv[1];
        port = (unsigned short) atoi(argv[2]);
        secret_key = (unsigned int) atoi(argv[3]);
        new_key = (unsigned int) atoi(argv[4]);

    if ((secret_key == 0) || (new_key == 0))
    {
        printf("Neither secret key can equal 0\n");
        exit(EXIT_FAILURE);
    }

        // If the response we get indicates a success, print a success message
        if(newKey(host, port, secret_key, new_key) == RESPONSE_SUCCESS)
        printf("success\n");
        else
        printf("failure\n");

        return EXIT_SUCCESS;
}
