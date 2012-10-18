# Makefile file
# Two mode
# make: output normal version execution file
# make debug: output debug version execution file
CC = gcc
CFLAGS = -O -Wall -ansi -lpthread -std=c99 -m64
DEBUG = -DDEBUG
myhttpd: main.o logging.o
	$(CC) $(CFLAGS) -o myhttpd main.o logging.o scheduling.o

main.o:  main.c
	$(CC) $(CFLAGS) -c main.c 

logging.o: logging.c
	$(CC) $(CFLAGS) -c logging.c

scheduling.o: scheduling.c
	$(cc) $(CFLAGS) -c scheduling.c
# For the debug mode
debug:
	$(CC) $(CFLAGS) $(DEBUG) main.c logging.c -o myhttpd -DDEBUG
