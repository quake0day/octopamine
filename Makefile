# Makefile file
# Two mode
# make: output normal version execution file
# make debug: output debug version execution file
CC = gcc
CFLAGS = -O -Wall -ansi
DEBUG = -DDEBUG
myhttpd: main.o logging.o
	$(CC) -o myhttpd main.o logging.o

main.o:  main.c
	$(CC) -c main.c 

logging.o: logging.c
	$(CC) -c logging.c

# For the debug mode
debug:
	$(CC) main.c logging.c -o myhttpd -DDEBUG
