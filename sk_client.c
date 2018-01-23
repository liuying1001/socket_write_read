
/*************************************************************************
	> File Name: sk_client.c
	> Author: Liuying
	> E-mail: liuying31195@126.com
	> Created Time: 2018.1.22
 ************************************************************************/

#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "sk_comm.h"

int main()
{

    struct sockaddr_in client_addr;
    bzero(&client_addr, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);
    client_addr.sin_port = htons(0);

    int client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket_fd < 0)
    {
        perror("Create Socket Failed:");
        exit(1);
    }

    if(-1 == (bind(client_socket_fd, (struct sockaddr*)&client_addr, sizeof(client_addr))))
    {
        perror("Client Bind Failed:");
        exit(1);
    }


    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) == 0)
    {
        perror("Server IP Address Error:");
        exit(1);
    }
    server_addr.sin_port = htons(SERVER_PORT);
    socklen_t server_addr_length = sizeof(server_addr);


    if(connect(client_socket_fd, (struct sockaddr*)&server_addr, server_addr_length) < 0)
    {
        perror("Can Not Connect To Server IP:");
        exit(0);
    }

    //Input the cmd and parse the cmd.
    int direction = -1;
    int slab;
    int nlab;
    char data[BUFFER_SIZE];
    char cmd[BUFFER_SIZE];
    bzero(cmd, BUFFER_SIZE);
    printf("Please Input cmd:  ");
    scanf("%[^\n]", cmd);
	char package[BUFFER_SIZE];
	bzero(package, BUFFER_SIZE);
	strncpy(package,cmd,strlen(cmd));

    if(parse_cmd_param(cmd, &direction,&slab,&nlab,data) != 0)
    {
        perror("Input command not support.\n");
        exit(1);
    }
    
    //send the cmd to server.
    if(send(client_socket_fd, package, BUFFER_SIZE, 0) < 0)
    {
        perror("Send File Name Failed:");
        exit(1);
    }

    
    /*
    *if cmd is write, will recv the server write result. mostly is "OK".
    *if cmd is read, will get the file data and print it to console.
    *fix me:
    *if file has hole, and read range cover it server will crash.
    */
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);
    int length = 0;


    while((length = recv(client_socket_fd, buffer, BUFFER_SIZE, 0)) >0)
    {
        if(direction == READ)
        {
           printf("Read file context: %s\n", buffer);            
        }
        else if(direction == WRITE)
        {
            buffer[length] = '\0';
            printf("Write Status: %s\n", buffer);
        }
        bzero(buffer, BUFFER_SIZE);
    }

    close(client_socket_fd);
    return 0;
}


