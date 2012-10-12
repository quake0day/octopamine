#include "main.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
/*
 * A minimal web server (version 1.0 alpha)
 * Si Chen
 * */

int DEBUG_MODE = 0; // DEBUG mode: off
int LOGGING = 0; // LOGGING mode : off
int HELP = 0; // HELP mode : off
int PORT = 8082; // httpd listening port, default is 8080
int TIME = 60; // queuing time(s), default is 60(s)
int THREADNUM = 4; // Number of threads in thread pool
int SCHED = 0; // scheduling policy FCFS=0 SJF=1, default is FCFS

void process_request(char *rq, int fd);
int request_arg_judge(char *f);
int request_file_type(char *f);
void provide_header(int file_type,int fd);
char show_date();
int main(int argc, char **argv)
{
	struct sockaddr_in sockaddr; /* incoming address */
	
	int socket_id; /* define socket id */
	
	
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
    logging();
	
	
	//initial socket here
	socket_id = socket(PF_INET,SOCK_STREAM,0); /*initial*/
	if(socket_id == -1) /* handle exceptions */
		return -1;
	
	
	//NEED CHANGE !!
	sockaddr.sin_port = htons(PORT); /* socket port */
	
	sockaddr.sin_family = AF_INET; /* addr family */
	if ( bind(socket_id,(struct sockaddr *) &sockaddr, sizeof(sockaddr)) != 0 ) // bind socket to cetrain addr
		return -1;
	
	/* listen for incomming calls */
	if(listen(socket_id,1) != 0)
		return -1;
	
	while(1)
	{
		/* take a call and save it to buffer */
		fd = accept(socket_id,NULL,NULL);
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
		close(fd);
		fclose(fpin);
	}
    return 0; // the end... die!!!!!!!!

}


/* skip over all request only concerned on CRNL */
find_crnl(FILE *fp)
{
	char buf[BUFSIZ];
	while(fgets(buf,BUFSIZ,fp) != NULL && strcmp(buf,"\r\n") !=0 );
}

/* handle the request 
** Do as the request asks for
** give them the reply
** THREADPPPPPPPOOOOL....
*/
void process_request(char *rq, int fd)
{
	int ARGS = 2; // define the arguments that one request should have.
	char arg_f[BUFSIZ];
	char cmd[BUFSIZ];
	int reqtype,filetype;
	//fork a new process to dealing this....
	// NEED change!
	if(fork() != 0)
		return;
	
	strcpy(arg_f,"./");  //process arguments starts with ./
	if(sscanf(rq, "%s %s",cmd, arg_f +2) != 2)
		return;
	
	if(strcmp(cmd, "GET") != 0)
		return; // broken??
	else
		reqtype = request_arg_judge(arg_f);
#ifdef DEBUG
	printf("cmd:%s\n",cmd);
	printf("arg_f:%s\n",arg_f);	
	printf("reqtype:%d\n",reqtype);
#endif
	switch(reqtype){
		case 0:
#ifdef DEBUG			
			printf("Now I'm in reqtype:0\n");
#endif
			filetype = request_file_type(arg_f);
			provide_header(filetype,fd);
#ifdef DEBUG			
			printf("filetype:%d\n",filetype);
#endif		
			
			break;
		case 1:
			printf("hihih 1\n");
			break;
		case 2:
			show_404(arg_f,fd);
			break;
		default:
			break;
	}
	

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

show_404(char *arg, int fd)
{
	FILE *fp = fdopen(fd,"w");
	
	fprintf(fp,"HTTP/1.0 404 Not Found\r\n");
	fprintf(fp,"Content-type:text/plain\r\n");
	fprintf(fp,"\r\n");
	fprintf(fp,"This file: %s\r\nis not found on our server\r\n",arg);
	fclose(fp);
}
/*
* Provide the reply header for html and gif if they exists
*/
void provide_header(int file_type,int fd)
{
	FILE *fp = fdopen(fd,"w");
	char *timebuf = NULL;
	/*
#ifdef DEBUG			
			printf("I'm in provide_header\n");
			printf("This file type is %d\n",file_type);
#endif		*/
	fprintf(fp,"HTTP/1.0 200 OK\r\n");
	*timebuf = show_date();
	fprintf(fp,"Date:%s",timebuf);
	/* If it is a html file */
	if(file_type == 0){
		fprintf(fp,"Content-type:text/html");	
	}
	/* If it is a gif file */
	if(file_type == 1){
		fprintf(fp,"Content-type:image/gif");
	}
	fclose(fp);
}
show_dir(char *dir, int fd)
{
	
}

char show_date()
{
 	char timebuf[100];
	/* Obtain current time as seconds elapsed since the Epoch. */
	time_t t;
	time(&t);
    /* Convert to 
	Sun, 06 Nov 1994 08:49:37 GMT    ; 
	RFC 822, updated by RFC 1123 */
	strftime(timebuf,sizeof(timebuf),"%a, %d %b %Y %H:%M:%S GMT",gmtime(&t));
	return *timebuf;
}
