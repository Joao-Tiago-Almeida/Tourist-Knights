CFLAGS	=	-g	-Wall	-ansi	-pedantic	-std=c99
#EXECUTABLE	=	tuktuk
#LIBS=	-lm
CC	=	gcc
#RC	=	$(wildcard	*.c)
#OBJ	=	$(SRC:	.c=.o)
#%.o: %.c

#$(EXECUTABLE):	$(OBJ)
#			$(CC)	-c	$(CFLAGS)
#			$(CC)	-o	$@	-c	$<	$(CFLAGS)
#
$(EXECUTABLE):	main.o	passeios.o	tabuleiro.o	util.o	vector2.o
#tuktuk:	main.o	passeios.o	tabuleiro.o	util.o	vector2.o
	$(CC)	-c	$(CFLAGS)	-o $(CFLAGS)	main.o	list.o	words.o	utils.o
main.o:main.c	passeios.h
	$(CC)	-c	$(CFLAGS)	main.c

passeios.o:passeios.c	passeios.h	util.h
	$(CC)	-c	$(CFLAGS)	passeios.c

tabuleiro.o:tabuleiro.c	util.h
	$(CC)	-c	$(CFLAGS)	tabuleiro.c

util.o:util.c	util.h
	$(CC)	-c	$(CFLAGS)	util.c

vector2.o:vector2.c	vector2.h
	$(CC)	-c	$(CFLAGS)	vector2.c

clean::
			rm	-f	*.o	core	a.out	tuktuk *~
