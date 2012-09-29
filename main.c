/*
 * A minimal web server (version 1.0 alpha)
 * Si Chen
 * */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char **argv)
{
    int kkk = 0;

    int DEBUG_MODE = 0; // DEBUG mode: off
    int HELP = 0; // HELP mode : off
    int LOGGING = 0; // LOGGING mode : off
    int PORT = 8080; // httpd listening port, default is 8080
    int TIME = 60; // queuing time(s), default is 60(s)
    int THREADNUM = 4; // Number of threads in thread pool
    int SCHED = 0; // scheduling policy FCFS=0 SJF=1, default is FCFS

    // NEED CHANGE
    char *DIR_R = NULL;
    char *TIME_R = NULL;
    char *PORT_R = NULL;
    char *LOGGING_PATH = NULL;
    char *THREADNUM_R = NULL;
    char *SCHED_R =NULL;

    // trace user choice
    int choice;

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
                break;
            case 'r':
                DIR_R = optarg;
                break;
            case 't':
                TIME_R = optarg;
                break;
            case 'n':
                kkk = 1;
                THREADNUM_R = optarg;
                break;
            case 's':
                SCHED_R = optarg;
                break;
            case '?':
                if(optopt == 'l')
                    fprintf(stderr,"Option -%c requires an argument.",optopt);
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
    if(kkk == 1){printf("FOR TEST::::THREADNUM: %d \n",strtol(THREADNUM_R,NULL,10));}
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

    return 0; // the end... die!!!!!!!!

}
