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
//	void* (*function)(void* arg);    //函数指针
	FUNC 			 function;
	void*                   arg;     //函数参数。
	struct _thpool_job_t* prev;     // 指向上一个节点
	struct _thpool_job_t* next;	    //指向下一个节点
 } thpool_job_t;
 
 /**
	定义一个工作队列
 **/
 
typedef struct thpool_job_queue{
	thpool_job_t*    head;            //队列头指针 
	thpool_job_t*    tail;			   // 队列末尾指针
	int              jobN;					  //任务数
	sem_t*           queueSem;			  //x信号量
}thpool_jobqueue; 

typedef struct thpool_t{
	pthread_t* thread_id; //< pointer to threads' ID 
int thread_count; //< amount of threads 
thpool_jobqueue* jobqueue; //< pointer to the job queue
}thpool_t;


typedef struct Thread{
    pthread_t thread_id;       // thread ID 
    long thread_count;       // # connections handled 
} Thread;

void logging();
int sjf(int client_socket_id);
int fcfs(int client_socket_id);
void thread_create(int i);
thpool_t* thpool_init(int threadNum);
void *thread_listen();
void *thread_schedule();
void *thread_exec();
