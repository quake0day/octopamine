# Makefile file
# Two mode
# make: output normal version execution file
# make debug: output debug version execution file
CC = gcc
CFLAGS = -O -Wall -ansi -lpthread -std=c99 -m64
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
debug: main.o logging.o scheduling.o
	$(CC) $(CFLAGS) $(DEBUG) -o myhttpd main.o logging.o scheduling.o

clean:
	@- $(RM) *.o
	@- $(RM) myhttpd
	@- echo "Data Cleansing Done.Ready to Compile"