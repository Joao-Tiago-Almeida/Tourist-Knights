#	Compiler
CC	=	gcc
#	Compiler Flags
CFLAGS	=	-g	-Wall	-ansi	-pedantic	-std=c99 -Wextra
#	Compiler Libraries
LIBS=	-lm
#	name of the Program
EXECUTABLE	=	tuktuk
#	Sources
SRC	=	$(wildcard	src/*.c)
#	Headers
HDRS	=	$(wildcard	src/*.h)
#	Objects
OBJ	=	$(SRC:.c=.o)

%.o: %.c HDRS
	$(CC) $(CFLAGS) -c $<

dirs = $(shell ls testfiles)

$(EXECUTABLE):	$(OBJ)
	$(CC)	-o $(EXECUTABLE)	$(OBJ)	$(LIBS)

v:
	valgrind --leak-check=full ./$(EXECUTABLE) testfiles/${file}

O3:
	$(CC) ${SRC} -O3 -Wall -o tuktuk $(LIBS)

clean::
			rm	-f	src/*.o	core	tuktuk  testfiles/*.walks	*~


# TODO comentar isto tudo
