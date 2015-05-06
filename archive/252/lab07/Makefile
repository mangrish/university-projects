OBJ = domatch.o match.o special.o
SRC = domatch.c match.c special.c

LIBS = stdlib

CC= gnuc

CDEFS =

CFLAGS = 

tags:	tags $(SRC)
	ctags $(SRC)

prog:	$(OBJ)
	$(CC) $(CFLAGS) -o prog $(OBJ)

match.o:	special.h 
domatch.o:	domatch.h special.h
special.o:	domatch.h special.h 

clean:
	/bin/rm -f $(OBJ) core

clobber:	clean
	/bin/rm -f  prog tags 
