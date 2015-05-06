#
# Makefile for project 1, unit 332, 1997.
#

CC   =		gcc
CFLAGS =	-Wall -O0 -g
PSRC =		proj1.c disk.c event.c
POBJ =		proj1.o	event.o disk.o
GSRC =		gen1.c
GOBJ =		gen1.o

proj1:		$(POBJ)
		$(CC) -o proj1 $(POBJ)

gen1:		$(GOBJ)
		$(CC) -o gen1 $(GOBJ)

clean:
		/bin/rm $(POBJ) $(GOBJ)

clobber:
		/bin/rm $(POBJ) proj1 $(GOBJ) gen1

proj1.o:	event.h disk.h
event.o:	event.h disk.h
disk.o:		disk.h
gen1.o:		std.h
