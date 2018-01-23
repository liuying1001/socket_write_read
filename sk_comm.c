#include "sk_comm.h"
#include<string.h>
#include<stdio.h>

const static char* write_direction = "write";
const static char* read_direction = "read";

/***********************************************************************
*
* Fuction Name:parse_cmd_param
* Description: parse the cmd to get cmd type, and slab/nlab,if cmd is write
               there should have data.
* Input:cmd
* Output1:dirction, two tpye, write to server or read from server.
* Output2:slba, start lba within operation.
* Output3:nlba, end lab within operation.
* Output4:data, private data within write cmd.
* Return:0 cmd is valid, -1 is invalid.

************************************************************************/
int parse_cmd_param(char* cmd, int* direction, int* slba, int* nlba, char* data)
{
    if(cmd == NULL)
    {
        return -1;
    }

    char* subcmd[4] = {NULL};
    char delims[] = " ";
    char* pdata = NULL;

    int i=0;
    subcmd[i] = strtok(cmd,delims);
	if(strcmp(subcmd[0],write_direction)!=0 && strcmp(subcmd[0],read_direction)!=0)
    {
    	perror("error: unsupport cmd\n");
		return -1;
    }

    while(subcmd[i] != NULL && i<=3)
    {
		i++;
        subcmd[i] = strtok(NULL,delims);
    }

    if(strcmp(subcmd[0],"read") == 0)
    {
        *direction = READ;
    }
    else if(strcmp(subcmd[0], "write") == 0)
    {
        *direction  = WRITE;
		if(NULL == subcmd[3])
	    {
	    	perror("write commnad need data parameter\n");
			return -1;
	    }
		strncpy(data,subcmd[3],strlen(subcmd[3]));
    }

    *slba = atoi(subcmd[1]);
    *nlba = atoi(subcmd[2]);
    if(*slba <0 || *nlba<0 || *nlba<*slba)
    {
        return -1;
    }

    return 0;
    
}
