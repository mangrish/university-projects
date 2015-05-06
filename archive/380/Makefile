# Makefile for 433-380 project for Semester 2, 2000
#
# This is a simple Makefile that builds 'proj'.

CC = gcc
CFLAGS = -Wall -pedantic
LIB = -lm

PROG = proj
SRCS = proj2.c read_off.c
HDRS = proj2.h read_off.h structs2.h
OBJS = proj2.o read_off.o


$(PROG):        $(OBJS)
				$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LIB)


proj.o:         proj2.h read_off.h structs2.h

read_off.o:     read_off.h structs2.h

clean:
				-rm -f $(OBJS)

clobber:        clean
				-rm -f $(PROG) 

