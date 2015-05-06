

owners:	owners.e gethotels.e getowners.e build.e owners.h 
	cp owners.e tmp.c
	cp gethotels.e tmp2.c
	cp getowners.e tmp3.c
	cp build.e tmp4.c
	cp /home/stude/data/351/proj2/dbname.h .
	gcc -E tmp.c > owners.sc
	gcc -E tmp2.c > gethotels.sc
	gcc -E tmp3.c > getowners.sc
	gcc -E tmp4.c > build.sc
	esqlc owners.sc gethotels.sc getowners.sc build.sc
	gcc -g -o owners owners.c gethotels.c getowners.c build.c -L $II_SYSTEM/ingres/lib -linterp.1 -lframe.1 -lq.1 -lcompat.1 -lm -lc -lsocket -lelf
	rm tmp*
