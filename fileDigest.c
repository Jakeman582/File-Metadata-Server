#include "csapp.h"
#include "file_server.h"

int main(int argc, char **argv) {

    // Declare local variables
        char *host;
        unsigned short port;
        unsigned int secret_key;
        char *file_name;
        char result[RESPONSE_LENGTH - RESPONSE_PAYLOAD_START];
        unsigned int result_length;

    // Verifiy argument count
    if (argc != 5)
    {
        fprintf(stdout, "Usage: %s host port secret fileName\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    host = argv[1];
    port = (unsigned short) atoi(argv[2]);
    secret_key = (unsigned int) atoi(argv[3]);
    file_name = argv[4];


    if (secret_key == 0)
        {
        printf("Secret key cannot equal 0\n");
            exit(EXIT_FAILURE);
        }

    if (fileDigest(host, port, secret_key, file_name, result, &result_length) != RESPONSE_SUCCESS)
    {
        fprintf(stdout, "failure\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("%s", result);
    }

    return 0;
}
