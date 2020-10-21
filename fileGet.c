#include "csapp.h"
#include "file_server.h"

int main(int argc, char **argv) {

    // Local variables
    int index;
    char *host;
    unsigned short port;
    unsigned int secret_key;
    char *file_name;
    char result[RESPONSE_LENGTH - RESPONSE_PAYLOAD_START];
    unsigned int result_length;

    // Make sure there are enough arguments
    if(argc != 5) {
        fprintf(
            stdout,
            "usage: %s host port secret_key file_name\n",
            argv[0]
        );
        exit(EXIT_FAILURE);
    }

    // Set networking and application data
    host = argv[1];
    port = (unsigned short) atoi(argv[2]);
    secret_key = (unsigned int) atoi(argv[3]);
    file_name = argv[4];

    if (secret_key == 0)
    {
        printf("Secret key cannot equal 0\n");
        exit(EXIT_FAILURE);
    }

    // If the response we get indicates a success, print a success message
    if(fileGet(host, port, secret_key, file_name, result, &result_length) == RESPONSE_SUCCESS)
    {
        for(index = 0; index < result_length; index++)
            printf("%02x", (unsigned char) result[index]);
        printf("\n");
    }
    else
        printf("failure\n");

    return EXIT_SUCCESS;

}
