//
//  main.h
//  octopamine
//
//  Created by quake0day on 10/28/12.
//  Copyright (c) 2012 quake0day. All rights reserved.
//

#ifndef octopamine_main_h
#define octopamine_main_h

#include <limits.h>
#include <inttypes.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>      /* uname() */
#include <semaphore.h>
#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <dirent.h>  //

#define MAXCLIENT 32

extern int DEBUG_MODE;
extern int LOGGING;
extern int HELP;
extern int PORT;
extern int TIME;
extern int THREADNUM;
extern int SCHED;

extern int client_schedule[MAXCLIENT];
extern int iput_req,iget_req;

extern int clientId[MAXCLIENT];
extern int itail,ihead;

pthread_mutex_t clientId_mutex;
pthread_cond_t clientId_cond;
pthread_mutex_t clientId_req_mutex;
pthread_cond_t clientId_req_cond;

typedef enum __bool { false = 0, true = 1, } bool;

typedef void* (*FUNC)(void* arg);

typedef struct _thpool_job_t{
    FUNC 			 function;
    void*                   arg;     //function parameter
    int 			 socket_client_ID;
    struct _thpool_job_t* prev;     // aim to the previous node
    struct _thpool_job_t* next;	    // aim to the next node
} thpool_job_t;

/**
  Define a working queue
 **/

typedef struct thpool_job_queue{
    thpool_job_t*    head;            // head of the queue
    thpool_job_t*    tail;			   // tail of the queue
    int              jobN;					  // number of jobs
    sem_t*           queueSem;			  // x sem
}thpool_job_queue;

typedef struct thpool_t{
    pthread_t* thread_id; //< pointer to threads' ID
    int thread_count; //< amount of threads
    thpool_job_queue* jobqueue; //< pointer to the job queue
}thpool_t;


typedef struct Thread{
    pthread_t thread_id;       // thread ID
    long thread_count;       // # connections handled
} Thread;

void get_file(int fd,char *f);
void logging(char *, char *,char *,char *, char *);
//int sjf(int client_socket_id);
//int fcfs(int client_socket_id);
void thread_create(int i);
thpool_t* thpool_init(int threadNum);
int thpool_jobqueue_init(thpool_t* thread_p);
void *thread_listen();
void *thread_schedule();
void thread_exec(thpool_t* thread_p);


#endif
