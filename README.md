# octopamine
![Mou icon](http://www.trc-canada.com/Structures/O239750.png)

## Overview
**octopamine**, A simple http server.

For CSE 521 First Project

**Due Date: OCT 29 2012**

## Teammate
Si Chen

Divya Chaudhary	

Suqiang Chen

## How to compile
In octopamin folder, type
	
	make
Or if you like, you can make a debug version by typing
	
	make debug
And we also provide 
	
	make clean
to clean all outputs.

## How to run the server
You can run the server as a daemon process in the background or as a normal program. The default choice is run as a daemon process.

Here is the SYNOPSIS:

	myhttpd [-d] [-h] [-l file] [-p port] [-r dir] [-t time] [-n threadnum] [-s sched]

*Where:*
****

−d        : Enter debugging mode. That is, do not daemonize, only accept one connection at a time and enable logging to stdout. Without this option, the web server should run as a daemon process in the background.

−h        : Print a usage summary with all options and exit.

−l file   : Log all requests to the given file. See LOGGING for details.

−p port : Listen on the given port. If not provided, myhttpd will listen on port 8080.

−r dir : Set the root directory for the http server to dir.

−t time  : Set the queuing time to time seconds. The default should be 60 seconds.

−n threadnum: Set number of threads waiting ready in the execution thread pool to threadnum. The default should be 4 execution threads.

−s sched : Set the scheduling policy. It can be either FCFS or SJF. The default will be FCFS. (for example "myhttpd -s sjf")

****

## How to test
You can test all the server function by visting the follwoing url

* http://localhost:8080/

* http://localhost:8080/test.gif

* http://localhost:8080/1.html

* http://localhost:8080/1/

* http://localhost:8080/aaa/

For SJF part, you can also use this files to test. (We already tested it)

## More
We are using github and this project is host in github (a private repo). 