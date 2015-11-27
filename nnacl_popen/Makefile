#
PROG	=	nnacl_popen
OBJS	=	main.o
#
CC 	=	gcc
CFLAGS	=	-std=gnu11 -O2 -W -Wshadow -pedantic -Wconversion
LINK	=	-Wl,-s

.c.o:
	$(CC) -c $*.c $(CFLAGS)

$(PROG): $(OBJS) 
	$(CC) -o $(PROG) $(OBJS) $(LINK) $(LIBS)

clean:
	rm -f *.o
