//
//  scheduling.c
//  octopamine
//
//  Created by quake0day on 10/28/12.
//  Copyright (c) 2012 quake0day. All rights reserved.
//
#include "main.h"
#include <sys/stat.h>

/*

int fcfs(int client_socket_id)
{
    client_schedule[iput_req] = client_socket_id;
    iput_req++;
    if (iput_req == MAXCLIENT)
        iput_req = 0;
    if (iput_req == iget_req)
    {
        exit(1);
    }
    return 1;
}


int sjf(int client_socket_id)
{
    char request[BUFSIZ];
	char arg_f[BUFSIZ];
	int file_size[BUFSIZ];
	int i,j;
	FILE *fd;
    client_schedule[iput_req++] = client_socket_id;
    int socketid = client_schedule[iput_req];
    fd=fdopen(socketid,"r");
	fgets(request,BUFSIZ,fd);
	sscanf(request,"%*[^/]/%[^ ]",arg_f);
	FILE *arg_file = fopen(arg_f,"r");
	struct stat info;
	stat(arg_f,&info);
	file_size[iput_req++]=(int)info.st_size;
	for(i=0;i<iput_req-1;i++)
	{
		for(j=0;j<iput_req-i-1;j++)
		{
			if(file_size[j+1]<file_size[j])
			{
				int temp;
				temp=client_schedule[j];
				client_schedule[j]=client_schedule[j+1];
				client_schedule[j+1]=temp;
			}
		}
	}
	fclose(arg_file);
	if (iput_req == MAXCLIENT)
        iput_req = 0;
    if (iput_req == iget_req)
    {
        exit(1);
    }
    return 1;
}
 */
