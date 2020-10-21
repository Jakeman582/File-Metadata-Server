#include "csapp.h"
#include "file_server.h"

int newKey(char *machineName, unsigned short port, unsigned int secretKey, unsigned int newKey) {

    // Local variables
    unsigned int * bufferSecretKey;
    unsigned short * bufferCommand;
    unsigned int * bufferNewKey;
    char request[REQUEST_LENGTH], response[RESPONSE_LENGTH];
    int clientfd;
    rio_t rio;

    // Open connection to the filed server
    clientfd = Open_clientfd(machineName, port);
    Rio_readinitb(&rio, clientfd);

    // Create the request
    memset(request, REQUEST_CLEAR_CHARACTER, REQUEST_LENGTH);

    bufferSecretKey = (unsigned int *) &request[REQUEST_KEY_START];
    *bufferSecretKey = htonl(secretKey);

    bufferCommand = (unsigned short *) &request[REQUEST_COMMAND_START];
    *bufferCommand = htons(NEW_KEY);

    bufferNewKey = (unsigned int *) &request[REQUEST_PAYLOAD_START];
    *bufferNewKey = htonl(newKey);

    // Send the request
    Rio_writen(clientfd, request, REQUEST_LENGTH);

    // Receive the response
    Rio_readnb(&rio, response, RESPONSE_LENGTH);

    // Close the connection
    Close(clientfd);

    // Return a one-byte code indicating request success or failure
    return ntohl(response[RESPONSE_RETURN_CODE_START]);

}

int fileGet(char *machineName, unsigned int port, unsigned int secretKey, const char *fileName, char *result, unsigned int *resultLength) {

    // Local variables
    unsigned int *bufferSecretKey;
    unsigned short *bufferCommand;
    unsigned short *bufferLength;
    char *bufferFile;
    char request[REQUEST_LENGTH];
    char response[RESPONSE_LENGTH];
    int clientfd;
    rio_t rio;

    // Open connection to the filed server
    clientfd = Open_clientfd(machineName, port);
    Rio_readinitb(&rio, clientfd);

    // Create the request
    memset(request, REQUEST_CLEAR_CHARACTER, REQUEST_LENGTH);

    bufferSecretKey = (unsigned int *) &request[REQUEST_KEY_START];
    *bufferSecretKey = htonl(secretKey);

    bufferCommand = (unsigned short *) &request[REQUEST_COMMAND_START];
    *bufferCommand = htons(FILE_GET);

    bufferFile = (char *) &request[REQUEST_PAYLOAD_START];
    strncpy(bufferFile, fileName, strlen(fileName));
    request[REQUEST_LENGTH - 1] = '\0';

    // Send the request
    Rio_writen(clientfd, request, REQUEST_LENGTH);

    // Receive the response
    Rio_readnb(&rio, response, RESPONSE_LENGTH);

    // Close the connection
    Close(clientfd);

    // Parse out information from the response
    bufferLength = (unsigned short *) &response[RESPONSE_LENGTH_START];
    *resultLength = ntohs(*bufferLength);
    memcpy(result, &response[RESPONSE_PAYLOAD_START], RESPONSE_LENGTH - RESPONSE_PAYLOAD_START);

    // Return a one-byte code indicating request success or failure
    return ntohl(response[RESPONSE_RETURN_CODE_START]);

}

int fileDigest(char* machineName, unsigned int port, unsigned int secretKey, const char* fileName, char* result, unsigned int* resultLength)
{
    unsigned int * bufferSecretKey;
    unsigned short * bufferCommand;
    unsigned short * bufferLength;
    char * bufferFile;
    char request[REQUEST_LENGTH], response[REQUEST_LENGTH];
    int client_fd;
    rio_t rio;

    // Open connection to the filed server
    client_fd = Open_clientfd(machineName, port);
    Rio_readinitb(&rio, client_fd);

    // Create request
    memset(request, REQUEST_CLEAR_CHARACTER, REQUEST_LENGTH);

    bufferSecretKey = (unsigned int*) &(request[REQUEST_KEY_START]);
    *bufferSecretKey = htonl(secretKey);

    bufferCommand = (unsigned short*) &(request[REQUEST_COMMAND_START]);
    *bufferCommand = htons(FILE_DIGEST);

    bufferFile = (char*) &(request[REQUEST_PAYLOAD_START]);
    strncpy(bufferFile, fileName, strlen(fileName));
    request[REQUEST_LENGTH - 1] = '\0';

    // Send the request
    Rio_writen(client_fd, request, REQUEST_LENGTH);

    // Recieve the response
    Rio_readnb(&rio, response, RESPONSE_LENGTH);

    // Close the connection
    Close(client_fd);

    // Parse out information from the reponse
    bufferLength = (unsigned short*) &(response[RESPONSE_LENGTH_START]);
    *resultLength = ntohs(*bufferLength);
    memcpy(result, &response[RESPONSE_PAYLOAD_START], (RESPONSE_LENGTH - RESPONSE_PAYLOAD_START));

    // Return a one-byte code indicating request success or failure
    return ntohl(response[RESPONSE_RETURN_CODE_START]);
}

void handle_request(int connfd, unsigned int *secret_key)
{
        // Local variables
        unsigned short *command;
        unsigned int current_key;
        unsigned int *client_key;
        char request[REQUEST_LENGTH];
        char response[RESPONSE_LENGTH];
        rio_t rio;

        // Clear out the response buffer, presuming that the request is authorized
        // and successful
        memset(response, RESPONSE_CLEAR_CHARACTER, RESPONSE_LENGTH);
        response[RESPONSE_RETURN_CODE_START] = (char) RESPONSE_SUCCESS;

        // Read the request from the client
        Rio_readinitb(&rio, connfd);
        Rio_readnb(&rio, request, REQUEST_LENGTH);

        // If the key provided by the client matches the server's secret key,
        // process the request, reject otherwise
        client_key = (unsigned int *) &request[REQUEST_KEY_START];
    if (ntohl(*client_key) == *secret_key)
        {
            // Get the requested command and execute it.
            command = (unsigned short *) &request[REQUEST_COMMAND_START];
            switch(ntohs(*command))
            {
                case NEW_KEY:
                    current_key = *secret_key;
                    handle_new_key(secret_key, request, response);
                    print_request_key(request, &current_key, "newKey", "success");
                    break;
                case FILE_GET:
                    handle_file_get(request, response);
                    print_request_file(request, response, "fileGet");
                    break;
               case FILE_DIGEST:
                    handle_file_digest(request, response);
                    print_request_file(request, response, "fileDigest");
                    break;
                default:
                    printf("Unsupported request\n");
                    response[RESPONSE_RETURN_CODE_START] = (char) RESPONSE_FAILURE;
                    break;
            }
        }
        else
    {
        // The client does not know the secret key.
        // Document the request and indicate failure.
            response[RESPONSE_RETURN_CODE_START] = RESPONSE_FAILURE;
            command = (unsigned short *) &(request[REQUEST_COMMAND_START]);

            switch(ntohs(*command))
        {
                case NEW_KEY:
            current_key = ntohl(*client_key);
            print_request_key(request, &current_key, "newKey", "failure");
            break;
                case FILE_GET:
                    print_request_file(request, response, "fileGet");
                    break;
                case FILE_DIGEST:
                    print_request_file(request, response, "fileDigest");
                    break;
                default:
                    printf("Unsupported request\n");
                    break;
            }
        return;
        }
        // Send a response to the client.
        Rio_writen(connfd, response, RESPONSE_LENGTH);
}

void handle_new_key(unsigned int *secret_key, char *request, char *response)
{
        // Local variables
        unsigned int * new_key;

        // Extract the new key from the request, and put it into the server's secret key
        new_key = (unsigned int *) &request[REQUEST_PAYLOAD_START];
        *secret_key = (unsigned int) ntohl(*new_key);
}

void handle_file_get(char *request, char *response)
{
    // Local variables
        unsigned short bytes_read;
        unsigned short *response_length;
        char *file_name;
        FILE *cache;

        // Form the command to execute
        file_name = &request[REQUEST_PAYLOAD_START];

    // Capture the output of the file
        if (access(file_name, R_OK) < 0)
    {
        perror(file_name);
        response[RESPONSE_RETURN_CODE_START] = RESPONSE_FAILURE;
        return;
    }
    cache = Fopen(file_name, "r");

    if (cache == NULL)
    {
           response[RESPONSE_RETURN_CODE_START] = RESPONSE_FAILURE;
            Fclose(cache);
           return;
       }

        bytes_read = (unsigned short) Rio_readn(fileno(cache), &response[RESPONSE_PAYLOAD_START], RESPONSE_LENGTH - RESPONSE_PAYLOAD_START);
        response_length = (unsigned short *) &response[RESPONSE_LENGTH_START];
        *response_length = (unsigned short) htons(bytes_read);

        Fclose(cache);
}

void handle_file_digest(char* request, char* response)
{
    // Local variables
    unsigned short * response_length;
    char * command = malloc(MAX_DIGEST_LENGTH);

    strcat(command, "/usr/bin/sha256sum ");
    strncat(command, &request[REQUEST_PAYLOAD_START], (REQUEST_LENGTH - REQUEST_PAYLOAD_START));

    char * temporary_file = "/tmp/file_server.out";
    pid_t process_id = fork();
    if (process_id != 0)
        process_id = waitpid(process_id, NULL, 0);
    else
    {
        FILE * tmp_file_ptr = Fopen(temporary_file, "w");

        int file_fd = fileno(tmp_file_ptr);
        if (file_fd < 0)
        {
            perror(temporary_file);
            exit(EXIT_FAILURE);
        }
        dup2(file_fd, 1);

        Fclose(tmp_file_ptr);

        int run_check = system(command);
        if (run_check < 0)
        {
            perror(temporary_file);
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }

      FILE * tmp_file_ptr = Fopen(temporary_file, "r");

      int file_fd = fileno(tmp_file_ptr);
      if (file_fd < 0)
      {
        response[RESPONSE_RETURN_CODE_START] = RESPONSE_FAILURE;
        return;
      }

    if (fgets(&response[RESPONSE_PAYLOAD_START], MAX_DIGEST_OUTPUT, tmp_file_ptr) != NULL)
    {
        response_length = (unsigned short*) &(response[RESPONSE_LENGTH_START]);
        int i = RESPONSE_PAYLOAD_START;
        while (response[i] != '\0')
        {
            i++;
            (*response_length)++;
        }
    }
    else
        response[RESPONSE_RETURN_CODE_START] = RESPONSE_FAILURE;

    Fclose(tmp_file_ptr);
}

void print_request_key(char *request, unsigned int *current_key, const char *type, const char *status)
{
        unsigned int * key = (unsigned int *) &request[REQUEST_PAYLOAD_START];
        printf("Secret key   = %u\n", *current_key);
        printf("Request type = %s\n", type);
        printf("Detail       = %u\n", ntohl(*key));
        printf("Completion   = %s\n", status);
        printf("--------------------------\n");

}

void print_request_file(char *request, char *response, const char *type)
{
      unsigned int * key = (unsigned int *) &request[REQUEST_KEY_START];
      printf("Secret key   = %u\n", ntohl(*key));
      printf("Request type = %s\n", type);
      printf("Detail       = %s\n", &request[REQUEST_PAYLOAD_START]);
      if(response[RESPONSE_RETURN_CODE_START] == RESPONSE_SUCCESS)
          printf("Completion   = success\n");
        else
            printf("Completion   = failure\n");
        printf("--------------------------\n");

}
