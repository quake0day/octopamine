# Makefile file
# Two mode
# make: output normal version execution file
# make debug: output debug version execution file
CC = gcc
CFLAGS = -O -Wall -ansi
myhttpd: main.o
	$(CC) -o myhttpd main.o 

main.o:  main.c
	$(CC) -c main.c 

# For the debug mode
debug:
	$(CC) main.c -o myhttpd -DDEBUG
