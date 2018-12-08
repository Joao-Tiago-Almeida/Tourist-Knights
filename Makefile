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
#	cria os fichiros *.o
%.o: %.c HDRS
	$(CC) $(CFLAGS) -c $<

#TODO
#não sei oq isto é
dirs = $(shell ls testfiles)

#gera o executável
$(EXECUTABLE):	$(OBJ)
	$(CC)	-o $(EXECUTABLE)	$(OBJ)	$(LIBS)

#corre o valgrind para file="<nome_ficheiro>.cities"
v:
	valgrind --leak-check=full ./$(EXECUTABLE) testfiles/${file}

#compila o código mais rapidamente
O3:
	$(CC) ${SRC} -O3 -Wall -o tuktuk $(LIBS)

#apaga ficheiros referidos na expressão
clean::
			rm	-f	src/*.o	core	tuktuk  testfiles/*.walks	*~
