CC	=	gcc
CFLAGS	=	-g	-Wall	-ansi	-pedantic	-std=c99
EXECUTABLE	=	tuktuk
LIBS=	-lm
SRC	=	$(wildcard	*.c)
OBJ	=	$(SRC:	.c=.o)
%.o: %.c

# main.o	passeios.o	tabuleiro.o	util.o vector2.o == $(OBJ)
$(EXECUTABLE):	$(OBJ)
	$(CC)	$(CFLAGS)	$(LIBS)	-o $(EXECUTABLE)	$(OBJ)


clean::
			rm	-f	*.o	core	a.out	tuktuk *~
