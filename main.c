#include "main.h"
#include <sys/socket.h>

/*
 * A minimal web server (version 1.0 alpha)
 * By:
 * Divya Chaudhary
 * Si Chen
 * Suqiang Chen
 * New version
 * */


int DEBUG_MODE = 0; // DEBUG mode: off
int LOGGING = 0; // LOGGING mode : off
int HELP = 0; // HELP mode : off
int PORT = 8080; // httpd listening port, default is 8080
int TIME = 60; // queuing time(s), default is 60(s)
int THREADNUM = 5; // Number of threads in thread pool
int SCHED = 0; // scheduling policy FCFS=0 SJF=1, default is FCFS

int client_schedule[MAXCLIENT];
int iput_req,iget_req;

thpool_t* threadpool; /* make a new thread pool structure */

int clientId[MAXCLIENT];
int itail,ihead;

void process_request(char *rq, int fd);
int request_arg_judge(char *f);
int request_file_type(char *f);
void provide_header(int file_type,int fd,char *f);
char *show_date();
int find_crnl(FILE *fp);
int show_404(char *arg, int fd);
int show_dir(char *dir, int fd);
int thpool_jobqueue_clean(thpool_t* thread_p);
int show_job_queue(thpool_t* thread_p);

// multithreading params init
Thread *t_ptr;
int socket_id,socket_client_id; /* define socket id */

static struct in_addr cli_addr; //structure for client address;
char *client_addr;
int length=0;
char *request_queuing_time=0;
char *request_scheduling_time=0;
//function to print time in correct format
char  *str=0;


char timebuf[1000];

//method for getting current time
char* get_time()
{
    //   char date[5], mon[5], year[5], hour[5], min[5], sec[5];
    time_t time_now;
    struct tm *time_local;
    time_now = time(0);
    time_local = localtime(&time_now);
    str=asctime(time_local);
    return str;
}

static int thpool_live = 1;

// main
int main(int argc, char **argv)
{
    struct sockaddr_in sockaddr; /* incoming address */

    //int socket_client_id; /* define socket id */


    // NEED CHANGE
    char *DIR_R = NULL;
    char *TIME_R = NULL;
    char *PORT_R = NULL;
    char *LOGGING_PATH = NULL;
    char *THREADNUM_R = NULL;
    char *SCHED_R =NULL;

    // trace user choice
    int choice;

    // handle request
    int fd;
    FILE *fpin;
    char request[BUFSIZ];


    while((choice = getopt(argc,argv,"dhl:p:r:t:n:s:")) != -1)
    {
        switch(choice){
            case 'd':
                DEBUG_MODE = 1;
                break;
            case 'h':
                HELP = 1;
                break;
            case 'l':
                LOGGING = 1;
                LOGGING_PATH = optarg;
                break;
            case 'p':
                PORT_R = optarg;
                PORT = atoi(PORT_R);
                break;
            case 'r':
                DIR_R = optarg;
                break;
            case 't':
                TIME_R = optarg;
                break;
            case 'n':
                THREADNUM_R = optarg;
                break;
            case 's':
                printf(" enter 0 for FCFS and 1 for SJF");
                SCHED_R = optarg;
                break;
            case '?':
                if(optopt == 'l')
                    fprintf(stderr,"Option -l requires an argument.");
                else
                    fprintf(stderr,"Unknown option");
                return 1;
            default:
                abort();

        }
    }
#ifdef DEBUG
    printf("FOR TEST::::DIR: %s \n",DIR_R);
    printf("FOR TEST::::TIME: %s \n",TIME_R);
    printf("FOR TEST::::LOGGING_PATH: %s \n",LOGGING_PATH);
    printf("FOR TEST::::LOGGING??: %d \n",LOGGING);
    printf("FOR TEST::::PORT: %s \n",PORT_R);
    printf("FOR TEST::::THREADNUM: %s \n",THREADNUM_R);
    //   if(kkk == 1){printf("FOR TEST::::THREADNUM: %d \n",strtol(THREADNUM_R,NULL,10));}
    printf("FOR TEST::::SCHED: %s \n",SCHED_R);
#endif

    // If program run in the help mode
    // print usage summary
    // and exit....
    if(HELP == 1){
        printf("usage:myhttpd [-d] [-h] [-l file] -[p port]");
        printf("[-r dir] [-t time] [-n threadnum] [-s sched]\n");
        return 0;
    }
    DEBUG_MODE=1;
    //  logging();


    //initial socket here
    socket_id = socket(PF_INET,SOCK_STREAM,0); /*initial*/
    if(socket_id == -1) /* handle exceptions */
        return -1;


    //NEED CHANGE !!
    sockaddr.sin_port = htons(PORT); /* socket port */
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr.sin_family = AF_INET; /* addr family */
    if ( bind(socket_id,(struct sockaddr *) &sockaddr, sizeof(sockaddr)) != 0 ) // bind socket to cetrain addr
        return -1;

    /* listen for incomming calls */
    if(listen(socket_id,1) != 0)
        return -1;

    //multithreading

    t_ptr = calloc(THREADNUM, sizeof(Thread));
    ihead=itail=0;
    // Why create THREADNUM+2??

    /*
       Thread* plisten;
       plisten = (Thread*) malloc(sizeof(Thread));
       pthread_create(&plisten, NULL, &thread_listen, NULL);
       Thread* pschedule;
       pschedule = (Thread*) malloc(sizeof(Thread));
       pthread_create(&pschedule, NULL, &thread_schedule, NULL);
       threadpool=thpool_init(THREADNUM);
       pthread_join(pschedule,NULL);
       pthread_join(plisten,NULL);

*/

    /*
       for(int i=0 ;i< THREADNUM+2;i++) {
       thread_create(i);
    //fprintf(stderr,"hi\n");
    }

*/
    // thread_create(0);
    // thread_create(1);
    // thread_create(2);

    // Something worg... we not be able to use threadpool
    //multithreading:ends

    while(1)
    {
        // take a call and save it to buffer
        fd = accept(socket_id,NULL,NULL);
        client_addr = inet_ntoa(cli_addr);
        if(fd == -1)
            break;
        fpin = fdopen(fd,"r");

        // read request
        fgets(request,BUFSIZ,fpin);
#ifdef DEBUG
        printf("got one! request = %s",request);
#endif
        find_crnl(fpin);
        process_request(request,fd);
        //divya's part start from here
        //include this part in execution threads
        request_queuing_time=get_time(); //call this function from queuing thread
        request_scheduling_time=get_time();//call this function from scheduling thread

        if(LOGGING){
#ifdef DEBUG
            fprintf(stderr," I am going into logging\n");
#endif
            logging(LOGGING_PATH,client_addr,request_queuing_time,request_scheduling_time,request);
        }
        //divya's part end here
        close(fd);
        fclose(fpin);
    }


    return 0; // the end... die!!!!!!!!

}


/* skip over all request only concerned on CRNL */
int find_crnl(FILE *fp)
{
    char buf[BUFSIZ];
    while(fgets(buf,BUFSIZ,fp) != NULL && strcmp(buf,"\r\n") !=0 );
    return 0;
}

/* handle the request
 ** Do as the request asks for
 ** give them the reply
 */
void process_request(char *rq, int fd)
{
    FILE *fp=fdopen(fd,"w");
    //int ARGS = 2; // define the arguments that one request should have.
    char arg_f[BUFSIZ];
    char cmd[BUFSIZ];
    int reqtype;
    int filetype;
    //fork a new process to dealing this....
    // NEED change!
    if(fork() != 0)
        return;
    
    strcpy(arg_f,"./");  //process arguments starts with ./
    if(sscanf(rq, "%s %s",cmd, arg_f +2) != 2)
        return;
    if(sscanf(rq,"%s",cmd)!= 0 && sscanf(rq,"%*[^/]/%[^ ]",arg_f)!= 0)
    {
        if(strcmp(cmd,"GET") == 0)
        {
            reqtype = request_arg_judge(arg_f);
#ifdef DEBUG
            printf("cmd:%s\n",cmd);
            printf("arg_f:%s\n",arg_f);
            printf("reqtype:%d\n",reqtype);
#endif
            switch(reqtype)
            {
                case 0:
#ifdef DEBUG
                    printf("Now I'm in reqtype:0\n");
#endif
                    filetype = request_file_type(arg_f);
                    
#ifdef DEBUG
                    printf("filetype:%d\n",filetype);
#endif
                    provide_header(filetype,fd,arg_f);
                    get_file(fd,arg_f);
                    break;
                case 1:
                    printf("I'm now in the directory\n");
                    show_dir(arg_f,fd);
                    break;
                case 2:
                    show_404(arg_f,fd);
                    break;
                default:
                    break;
            }
        }
        else if(strcmp(cmd,"HEAD") == 0)
        {
            filetype = request_file_type(arg_f);
#ifdef DEBUG
            printf("filetype:%d\n",filetype);
#endif
            provide_header(filetype,fd,arg_f);
        }
        else if(strcmp(cmd,"~") == 0)
        {
            char buf[50];
            getcwd(buf, sizeof(buf));
            fprintf(fp,"%s\n", buf);
        }
        else
            fprintf(fp,"request wrong!");
    }
    fclose(fp);
}

int request_arg_judge(char *f)
{
    struct stat info;
    /* define request type
     * 0 for normal file (means file exist and it is a file :)
     * 1 for directory (means it is a folder)
     * 2 for non (means found nothing..)
     */
    int req_type = 0;
    // judge if it is a directory
    if(stat(f,&info) != -1 && S_ISDIR(info.st_mode))
        req_type = 1;
    // judge if the item does not exist
    if(stat(f,&info) == -1)
        req_type = 2;
    return req_type;
}
int request_file_type(char *f)
{
    char *cp;
    /* filetype
     * 0 for html file
     * 1 for gif picture
     */
    int file_type = -1;
    if((cp = strrchr(f,'.')) != NULL)
        cp = cp +1;
    if(strcmp(cp,"html") == 0)
        file_type = 0;
    else if(strcmp(cp,"gif") == 0)
        file_type = 1;
    return file_type;
}

int show_404(char *arg, int fd)
{
    FILE *fp = fdopen(fd,"w");

    fprintf(fp,"HTTP/1.0 404 Not Found\r\n");
    fprintf(fp,"Content-type:text/plain\r\n");
    fprintf(fp,"\r\n");
    fprintf(fp,"This file: %s\r\nis not found on our server\r\n",arg);
    fclose(fp);
    return 0;
}
/*
 * Provide the reply header for html and gif if they exists
 */
void provide_header(int file_type,int fd,char *f)
{
    // note that fd represent socket id
	// f represent the request file's name
	// file_type means file_type...
	
	FILE *fp = fdopen(fd,"w");
    FILE *arg_file = fopen(f,"r");
	struct stat info;
	stat(f,&info);
	char *timebuff = NULL;
	char *serverv = NULL;
	char *modtimebuf = NULL;
	int file_size;
    /*
     #ifdef DEBUG
     printf("I'm in provide_header\n");
     printf("This file type is %d\n",file_type);
     #endif		*/
    // By Suqiang
    
    fprintf(fp,"HTTP/1.0 200 OK\r\n");
	timebuff = show_date();
	fprintf(fp,"Date:%s\n",timebuff);
	serverv = "A minimal web server (version 1.0 alpha)";
	fprintf(fp,"Server:%s\n",serverv);
	modtimebuf = ctime(&info.st_mtime);
	fprintf(fp,"Last-Modified:%s",modtimebuf);
	
    /* If it is a html file */
    if(file_type == 0){
        fprintf(fp,"Content-type:text/html\n");
    }
    /* If it is a gif file */
    if(file_type == 1){
        fprintf(fp,"Content-type:image/gif\n");
    }
	file_size = info.st_size;
	fprintf(fp,"Content-Length:%d\n\n",file_size);
    fclose(fp);
    fclose(arg_file);
}

// basic system & file info provide part
// By Suqiang Chen
// Modified by Si CHen

int show_dir(char *dir, int fd)
{
    FILE *fp = fdopen(fd,"w");
    DIR* dp;
    struct dirent *dirp;
    dp=opendir(dir);
    while ((dirp=readdir(dp))!=NULL)
        fprintf(fp,"%s\n ",dirp->d_name);
    closedir(dp);
    fclose(fp);
    return 0;
}

void get_file(int fd,char *f)
{
	FILE *fp, *arg_file;
	int c;
	fp=fdopen(fd,"w");
	arg_file=fopen(f,"r");
	if(fp!=NULL && arg_file!=NULL)
	{
		fprintf(fp,"\r\n");
		while((c=getc(arg_file))!=EOF)
		{
            putc(c,fp);
	    }
	    fclose(arg_file);
	    fclose(fp);
	}
	exit(0);
}



char *show_date() //good!
{
 	//char timebuf[1000];
 	struct tm *newtime;
	/* Obtain current time as seconds elapsed since the Epoch. */
	time_t t;
	time(&t);
	newtime=localtime(&t);
    /* Convert to
     Sun, 06 Nov 1994 08:49:37 GMT    ;
     RFC 822, updated by RFC 1123 */
	strftime(timebuf,sizeof(timebuf),"%a,%d/%b/%Y:%H:%M:%S GMT",newtime);
	return timebuf;
}





// multithreading part
// By Divya
static struct in_addr cli_addr; //structure for client address;
char *client_addr;

void thread_create(int i)
{
    if(i==0){
        pthread_create(&t_ptr[i].thread_id, NULL, &thread_listen, (void *)i);
    }
    else if(i==1){
        pthread_create(&t_ptr[i].thread_id, NULL, &thread_schedule, (void *)i);
    }
    else if (i == 2){
        threadpool=thpool_init(THREADNUM); /* initialise it to  number of threads */
        //pthread_create(&t_ptr[i].thread_id, NULL, &thread_exec, (void *)i);
    }
    /* main thread returns */
}

//this method will intialize the thread pool
thpool_t* thpool_init(int threadNum){
    thpool_t* pool_ptr;
    if(!threadNum || threadNum < 1)
        threadNum = 1;
    // allocate threadpool memory
    pool_ptr=(thpool_t*)malloc(sizeof(thpool_t));
    if (pool_ptr==NULL){
        fprintf(stderr, "thpool_init(): Could not allocate memory for thread pool\n");
        return NULL;
    }
    // allocate the number of threads
    pool_ptr->thread_count=threadNum;
    pool_ptr->thread_id=(pthread_t*)malloc(threadNum*sizeof(pthread_t)); /* MALLOC thread IDs */
    if (pool_ptr->thread_id==NULL){
        fprintf(stderr, "Malloc thpool -> threads error\n");
        return NULL;
    }

    /* need something here */

    /*
       typedef struct thpool_job_queue{
       thpool_job_t*    head;            // head of the queue
       thpool_job_t*    tail;			   // tail of the queue
       int              jobN;					  // number of jobs
       sem_t*           queueSem;			  // x sem
       }thpool_jobqueue;
       */


    if(thpool_jobqueue_init(pool_ptr))
        return -1;
    pool_ptr->jobqueue->queueSem = (sem_t*)malloc(sizeof(sem_t));
    sem_init(pool_ptr->jobqueue->queueSem,0,1);

    for (int t=0; t<threadNum; t++){
        fprintf(stderr,"Created thread %d in pool \n", t);
        pthread_create(&(pool_ptr->thread_id[t]), NULL, (void *)thread_exec, (void *)pool_ptr);
    }

    return pool_ptr;

}

// elimate a thread pool
void thpool_die(thpool_t* thread_p)
{
    int i;
    thpool_live = 0;
    for(i = 0; i < (thread_p -> thread_count); i++)
    {
        if(sem_post(thread_p -> jobqueue -> queueSem))
        {
            fprintf(stderr,"cannot destory thpool destory\n");
        }
    }
    if(sem_post(thread_p -> jobqueue -> queueSem) != 0)
    {
        fprintf(stderr,"cannot destory semaphore \n");
    }
    for(i = 0; i < (thread_p -> thread_count); i++)
    {
        pthread_join(thread_p -> thread_id[i],NULL);
    }
    thpool_jobqueue_clean(thread_p);

    free(thread_p -> thread_count);
    free(thread_p -> jobqueue-> queueSem);
    free(thread_p -> jobqueue);
    free(thread_p);
}

// clean a jobqueue

int thpool_jobqueue_clean(thpool_t* thread_p)
{
    // Point pinter to the end of it's job queue
    thpool_job_t* current_job;
    current_job = thread_p -> jobqueue -> tail;

    while(thread_p -> jobqueue -> jobN)
    {
        thread_p -> jobqueue -> tail = current_job -> prev;
        free(current_job);
        current_job = thread_p -> jobqueue -> tail;
        thread_p -> jobqueue -> jobN = thread_p -> jobqueue -> jobN - 1;

    }
    thread_p -> jobqueue -> head = NULL;
    thread_p -> jobqueue -> tail = NULL;
    
    return 0;

}

/* Initialise queue */
int thpool_jobqueue_init(thpool_t* thread_p){
    thread_p->jobqueue = (thpool_job_queue*)malloc(sizeof(thpool_job_queue));
    if(thread_p -> jobqueue == NULL) return -1;
    thread_p -> jobqueue -> head = NULL;
    thread_p -> jobqueue -> jobN = 0;
    thread_p -> jobqueue -> tail = NULL;

    return 0;
}
// get one node from queue one
thpool_job_t* thpool_jobqueue_peek(thpool_t* thread_p)
{
    // FCFS
    return thread_p->jobqueue->tail;
}

// delete the last node in queue
int thpool_jobqueue_removelast(thpool_t* thread_p)
{
    if(thread_p == NULL)
        return -1;
    thpool_job_t* lastjob;
    lastjob = thread_p->jobqueue->tail;
    switch(thread_p->jobqueue->jobN)
    {
        case 0:
            return -1;
        case 1:
            thread_p->jobqueue->head = NULL;
            thread_p->jobqueue->tail = NULL;
            break;
        default:
            lastjob->prev->next = NULL;
            thread_p->jobqueue->tail = lastjob->prev;
    }
    return 0;
}

// add one job to jobqueue
int thpool_jobqueue_addone(thpool_t* thread_p, thpool_job_t* newjob)
{
    // init
    newjob -> next = NULL;
    newjob -> prev = NULL;

    thpool_job_t* oldOne;

    oldOne = thread_p -> jobqueue -> head;

    if(thread_p -> jobqueue -> jobN == 0) // if it is an empty list
    {
        thread_p -> jobqueue -> head = newjob;
        thread_p -> jobqueue -> tail = newjob;
    }
    else
    {
        newjob -> next = oldOne;
        oldOne -> prev = newjob;
        thread_p -> jobqueue -> head = newjob;

    }
    thread_p -> jobqueue -> jobN = thread_p -> jobqueue -> jobN + 1;
    sem_post(thread_p -> jobqueue -> queueSem);

    int reval;
    sem_getvalue(thread_p -> jobqueue -> queueSem, &reval);
    
    return 0;

}

//int thpool_pool_addone(thpool_t* thread_p, void*)
void *thread_listen(){
    //this method(thread) will listen for the incoming requests and put them in a queue
    while(true){

        fprintf(stderr,"I'm in listener now");

        thpool_job_t* newjob;
        newjob = (thpool_job_t*) malloc(sizeof(thpool_job_t));
        if(newjob == NULL)
        {
            fprintf(stderr,"In listen, cannot allocate memory for new job\n");
        }
        int length=0;
        client_addr = inet_ntoa(cli_addr);
        length = sizeof(cli_addr);
        socket_client_id= accept(socket_id, (struct sockaddr *)&cli_addr, &length);
        if(socket_client_id< 0)
        {
            exit(1);
        }
        newjob -> socket_client_ID = socket_client_id;
        pthread_mutex_lock(&clientId_mutex);
        thpool_jobqueue_addone(threadpool,newjob);
        clientId[itail]=socket_client_id;
        itail++;
        if(itail==MAXCLIENT)
            itail=0;
        pthread_cond_signal(&clientId_cond);
        pthread_mutex_unlock(&clientId_mutex);
        show_job_queue(threadpool);
    }
}

int show_job_queue(thpool_t* thread_p)
{
    int jobNum;
    jobNum = thread_p -> jobqueue -> jobN;
    fprintf(stderr,"the number of job's in queue %d\n", jobNum);
    return 0;
}


void *thread_schedule(){
    //this method(thread) will pick the requests from the ready queue and schedule them according to policies
    //sleep(TIME);

    // sleep(TIME);

    while(true){
        pthread_cond_signal(&clientId_req_cond); //signal one of the execution threads
        fprintf(stderr,"I'm in scheduler now");

        pthread_mutex_lock(&clientId_mutex);
        /*
           while(ihead==itail){
        //   fprintf(stderr,"hello baby\n");
        pthread_cond_wait(&clientId_cond, &clientId_mutex);
        }
        while(ihead!=itail){
        if(SCHED==1){
        int value= sjf(clientId[ihead]);
        ihead=ihead+1;
        }
        else{
        int value= fcfs(clientId[ihead]);
        ihead=ihead+1;
        }
        if( ihead==MAXCLIENT){
        ihead=0;
        }
        }
        */

        pthread_mutex_unlock(&clientId_mutex);
        pthread_mutex_lock(&clientId_req_mutex);
        while(iput_req == iget_req)
            pthread_cond_wait(&clientId_req_cond,&clientId_req_mutex);

        pthread_cond_signal(&clientId_req_cond); //signal one of the execution threads

        pthread_mutex_unlock(&clientId_req_mutex);
    }

}

void thread_exec(thpool_t* thread_p){


    FILE *fpin;
    char request[BUFSIZ];




    //these are the threads which will execute different requests
    pthread_mutex_lock(&clientId_req_mutex);



    while(iput_req == iget_req)
    {
        pthread_cond_wait(&clientId_req_cond,&clientId_req_mutex);
    }
    fprintf(stderr,"I'm in exec\n");

    int socketid=client_schedule[iget_req]; //get request from scheduling queue
    fpin = fdopen(socketid,"r");
    // read request
    fgets(request,BUFSIZ,fpin);
#ifdef DEBUG
    printf("got one! request = %s",request);
#endif
    find_crnl(fpin);
    process_request(request,socketid);
    close(socketid);
    fclose(fpin);

    //do request part
    //coding needs to be done for request part


    pthread_mutex_unlock(&clientId_req_mutex);


}
