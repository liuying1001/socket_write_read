
#define SERVER_PORT 8000
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 4096
#define CMD_MAX_LEN 128
#define LAB_SIZE 4096
#define READ 0
#define WRITE 1

int parse_cmd_param(char* cmd, int* direction, int* slba, int* nlba,char* data);


