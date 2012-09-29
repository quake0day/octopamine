
//#include<time.h>
#include "main.h"

void logging()
{
//     int DEBUG_MODE = 1; // default DEBUG mode: off
//     time_t _t;
//    time(&_t);
     char *a=NULL;
     char *t=NULL;
     char *tt=NULL;
     char *r=NULL;
     char *s=NULL;
     char *b=NULL;
     a="127.0.0.1 ";      //zan shi shi jing tai de
//     t=(char*)ctime(&_t);
//     tt=(char*)ctime(&_t);
     t="[29/sep/2012:12:05:33 -0600] ";
     tt="[29/sep/2012:12:05:40 -0600] ";
     r="\"GET/index.html http/1.0\" ";
     s="200 ";
     b="1326 ";
     if(DEBUG_MODE==0)
     {
		 printf("%s %s %s %s %s %s\n",a,t,tt,r,s,b);
	 }
	 else
	 {
		 FILE *fp;
		 char *filepath=NULL;
		 filepath="log.txt";
		 char *str=NULL;
		 str=(char *)malloc(strlen(a)+strlen(t)+strlen(tt)+strlen(r)
		                    +strlen(s)+strlen(b)+2);
		 strcpy(str,a);
		 strcat(str,t);
		 strcat(str,tt);
		 strcat(str,r);
		 strcat(str,s);
		 strcat(str,b);
		 strcat(str,"\n");
		 fp=fopen(filepath,"a+");
		 if(fp==NULL)
		 {
			 return;
		 }
		 else
		 {
			 fputs(str,fp);
	     } 
	     fclose(fp);
	 }
}

