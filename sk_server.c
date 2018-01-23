
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include<fcntl.h>

#include "sk_comm.h"

int main(void)
{

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    const static char* file_name = "file.txt";

    int server_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(server_socket_fd < 0)
    {
        perror("Create Socket Failed:");
        exit(1);
    }
    int opt = 1;
    setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


    if(-1 == (bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))))
    {
        perror("Server Bind Failed:");
        exit(1);
    }
    
    if(-1 == (listen(server_socket_fd, LENGTH_OF_LISTEN_QUEUE)))
    {
        perror("Server Listen Failed:");
        exit(1);
    }

    while(1)
    {                  
        struct sockaddr_in client_addr;
        socklen_t client_addr_length = sizeof(client_addr);
        int new_server_socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_addr, &client_addr_length);

        if(new_server_socket_fd < 0)
        {
            perror("Server Accept Failed:");
            break;
        }

        int bufferlen = 0;
        char buffer[BUFFER_SIZE];
        char data[BUFFER_SIZE];
        bzero(buffer, BUFFER_SIZE);

        if(bufferlen = recv(new_server_socket_fd, buffer, BUFFER_SIZE, 0) < 0)
        {
            perror("Server Recieve Data Failed");
            break;
        }
        
        if(strlen(buffer)==0)
        {
            continue;
        }
        
        int direction = -1;
        int slba = -1;
        int nlba = -1;
        
        if(parse_cmd_param(buffer, &direction, &slba, &nlba, data) !=0)
        {
            perror("cmd parameter is wrong\n");
            break;
        }
        
        int fp = open(file_name, O_CREAT|O_RDWR);
        if(fp<0)
        {
            printf("File:%s Not Found\n", file_name);
        }

        
        int size  = lseek(fp,0L,SEEK_END);  
        const char* startpos_error = "\nerror:start lba over file length";
        if(slba*LAB_SIZE > size && direction == READ)
        {
            perror("start pos is over file length.\n");
            send(new_server_socket_fd, startpos_error, strlen(startpos_error), 0);
            close(new_server_socket_fd);
            continue;
        }

     
        lseek(fp,slba*LAB_SIZE,SEEK_SET);
        int blk_count = nlba - slba;
        int byte_count = blk_count*LAB_SIZE;
        if(READ == direction)
        {
            bzero(buffer, BUFFER_SIZE);
            int length = 0;
            
          while((length = read(fp, buffer, byte_count))>0)
            {
                if(send(new_server_socket_fd, buffer, length, 0) < 0)
                {
                    printf("Send File:%s Failed.\n", file_name);
                    break;
                }
                printf("read data from file:%s\n",file_name);
                bzero(buffer, BUFFER_SIZE);
                byte_count-=length;
            }
          
            if(byte_count == blk_count*LAB_SIZE)
            {
                send(new_server_socket_fd, "No data", strlen("No data"), 0);
            }
            
            close(fp);
        }
        else if(WRITE == direction)
        {
            if(write(fp,data, strlen(data)) < strlen(data))  
            {  
                printf("File:\t%s Write Failed\n", file_name);  
                break;  
            }  
            printf("write data to file:%s\n",file_name);
            bzero(buffer, BUFFER_SIZE); 
            close(fp);
            if(send(new_server_socket_fd, "OK",3 , 0) < 0)
            {
                printf("send write result to client failed\n");
            }            
        }   

        close(new_server_socket_fd);
        
    }

    close(server_socket_fd);
    return 0;
}
