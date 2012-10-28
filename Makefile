# Makefile file
# Two mode
# make: output normal version execution file
# make debug: output debug version execution file
CC = gcc
CFLAGS = -O -Wall -lpthread -std=c99 -pedantic -D_XOPEN_SOURCE=600 -pthread
DEBUG = -DDEBUG
myhttpd: main.o logging.o scheduling.o
	$(CC) $(CFLAGS) -o myhttpd main.o logging.o scheduling.o

main.o:  main.c
	$(CC) $(CFLAGS) -c main.c 

logging.o: logging.c
	$(CC) $(CFLAGS) -c logging.c

scheduling.o: scheduling.c
	$(CC) $(CFLAGS) -c scheduling.c

# For the debug mode
debug: 
	$(CC) $(CFLAGS) $(DEBUG) -o myhttpd main.c logging.c scheduling.c

clean:
	@- $(RM) *.o
	@- $(RM) myhttpd
	@- echo "Data Cleansing Done.Ready to Compile"
