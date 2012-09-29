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

extern int DEBUG_MODE;
extern int LOGGING;
extern int HELP;
extern int PORT;
extern int TIME;
extern int THREADNUM;
extern int SCHED;

void logging();
