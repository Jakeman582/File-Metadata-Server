// Transaction Constants
#define REQUEST_LENGTH  108
#define RESPONSE_LENGTH 106
#define MAX_DIGEST_LENGTH 120
#define MAX_DIGEST_OUTPUT 100

// Request buffer constants
#define REQUEST_KEY_START     0
#define REQUEST_COMMAND_START 4
#define REQUEST_PADDING_START 6
#define REQUEST_PAYLOAD_START 8

// Request status constants
#define REQUEST_CLEAR_CHARACTER '\0'

// Request command constants
#define NEW_KEY     0
#define FILE_GET    1
#define FILE_DIGEST 2

// Response buffer constants
#define RESPONSE_RETURN_CODE_START  0
#define RESPONSE_PADDING_START      1
#define RESPONSE_LENGTH_START       4
#define RESPONSE_PAYLOAD_START      6

// Response status constants
#define RESPONSE_SUCCESS          0
#define RESPONSE_FAILURE         -1
#define RESPONSE_CLEAR_CHARACTER '\0'

// Client request functions
int newKey(char* machineName, unsigned short port, unsigned int secretKey, unsigned int newKey);
int fileGet(char* machineName, unsigned int port, unsigned int secretKey, const char* fileName, char* result, unsigned int* resultLength);
int fileDigest(char* machineName, unsigned int port, unsigned int secretKey, const char* fileName, char* result, unsigned int* resultLength);
int fileRun(char* machineName, unsigned int port, unsigned int secretKey, const char* request, char* result, unsigned int* resultLength);

// Server utility functions
void handle_request(int connfd, unsigned int* secret_key);
void handle_new_key(unsigned int* secret_key, char* request, char* response);
void handle_file_get(char* request, char* response);
void handle_file_digest(char *request, char *response);
void print_request_key(char *request, unsigned int *current_key, const char *type, const char *status);
void print_request_file(char* request, char* response, const char* type);
