CC	=	gcc
CFLAGS	=	-g	-Wall	-ansi	-pedantic	-std=c99
EXECUTABLE	=	tuktuk
LIBS=	-lm
SRC	=	$(wildcard	src/*.c)
OBJ	=	$(SRC:	.c=.o)
%.o: %.c


$(EXECUTABLE):	$(OBJ)
	$(CC)	$(CFLAGS)	$(LIBS)	-o $(EXECUTABLE)	$(OBJ)


clean::
			rm	-f	*.o	core	tuktuk *~

# TODO comentar isto tudo
