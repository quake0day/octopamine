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

void logging(char *log_file, char *ip_addr, char *request_queuing_time, char *request_scheduling_time,char *req)
{
    char logbuf[1024];
    FILE *fp;
    char *filepath=NULL;
    filepath=log_file;
    char cmd[BUFSIZ];
    char arg_f[BUFSIZ];
    sscanf(req,"%s",cmd);
    sscanf(req,"%*[^/]/%[^ ]",arg_f);
   // FILE *arg_file = fopen(arg_f,"r");
    struct stat info;
    stat(arg_f,&info);
 //   int filesize;

 //   char *a=NULL;
    char *t=NULL;
    char *tt=NULL;
    int status;
    char *size=NULL;

    t= request_queuing_time;
//    if(t[strlen(t)]== '\n')   t[strlen(t)]= '\0 ';
    tt=request_scheduling_time;

    if (strcmp(cmd,"GET")==0)
    {

        status=200;
        size= info.st_size;

    }
    else
    {
        status=400;
        sprintf(size,"%d",0) ;
    }
    if(DEBUG_MODE==0)
    {
        (void)printf("%s - [%s] [%s] \"%s\" %d %d\n",ip_addr,t,tt,req,status,size);
    }

    else
    {

        (void)sprintf(logbuf,"%s [%s] [%s] \"%s\" %d %d\n",ip_addr,t,tt,req,status,size);
        if((fp=open(filepath,O_CREAT|O_WRONLY|O_APPEND,0644)) >= 0){
            (void)write(fp,logbuf,strlen(logbuf));
            //(void)write(fp,"\n",2);
            (void)close(fp);
        }
    }
}




