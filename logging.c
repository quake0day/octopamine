//
//  logging.c
//  octopamine
//
//  Created by quake0day on 10/28/12.
//  Copyright (c) 2012 quake0day. All rights reserved.
//


//#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<pthread.h>
#include<string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "main.h"

int BUFSIZE = 1024;

//memset(logbuf,0,100);
void logging(char *log_file, char *ip_addr, char *request_queuing_time, char *request_scheduling_time,char *req)
{
    char logbuf[1024]="0";
    memset(logbuf, 0, sizeof(logbuf));
    FILE *fp;
    char *filepath=NULL;
    filepath=log_file;
    char cmd[BUFSIZ];
    char arg_f[BUFSIZ];
    sscanf(req,"%s",cmd);
    sscanf(req,"%*[^/]/%[^ ]",arg_f);
    // FILE *arg_file = fopen(arg_f,"r");
    struct stat info;
    // stat(arg_f,&info);
    //   int filesize;

    //   char *a=NULL;
    char *t=NULL;
    char *tt=NULL;
    int status;
    //char *size=NULL;
    long size=0;
    if(stat(arg_f,&info) != -1){ // if file exists
        size= info.st_size;
        if(size >= 72340172838076672){
            status = 404;
            size = 0;
        }
    }
    else{
        size = 0;
    }

    if (strcmp(cmd,"GET")==0)
    {
        status=200;
    }
    else if(strcmp(cmd,"HEAD") == 0)
    {
        status=400;
        size = 0 ;
    }
    else
    {
        status=404;
        size = 0 ;
    }
    t= request_queuing_time;
    t = strtok(t, "\n");

    tt=request_scheduling_time;

    // delete \n from req
    req = strtok(req,"\r");


    if(DEBUG_MODE==1)
    {
        (void)printf("%s - [%s] [%s] \"%s\" %d %ld\n",ip_addr,t,tt,req,status,size);
    }

    else
    {

        //snprintf(logbuf,15,"%s [%s] [%s] \"%s\" %d %s\n",ip_addr,t,tt,req,status,size);
        snprintf(logbuf,1023,"%s - [%s] [%s] \"%s\" %d %ld\n",ip_addr,t,tt,req,status,size);


        if((fp=open(filepath,O_CREAT|O_WRONLY|O_APPEND,0644)) >= 0){
            (void)write(fp,logbuf,strlen(logbuf));
            //(void)write(fp,"\n",2);
            (void)close(fp);
        }
    }
}




