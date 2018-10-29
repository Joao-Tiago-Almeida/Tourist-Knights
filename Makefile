#	Compiler
CC	=	gcc
#	Compiler Flags
CFLAGS	=	-g	-Wall	-ansi	-pedantic	-std=c99
#	Compiler Libraries
LIBS=	-lm
#	name of the Program
EXECUTABLE	=	tuktuk
#	Sources
SRC	=	$(wildcard	src/*.c)
#	Objects
OBJ	=	$(SRC:	.c=.o)

#se eu apagr isto não era suposto dar!
#%.o: %.c


$(EXECUTABLE):	$(OBJ)
	$(CC)	$(CFLAGS)	$(LIBS)	-o $(EXECUTABLE)	$(OBJ)


clean::
			rm	-f	*.o	core	tuktuk *~

# TODO comentar isto tudo
