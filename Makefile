TARGET = executable
LIBS = -lm
CC = gcc src/*.c
CFLAGS = -g -Wall -ansi -pedantic -std=c99

.PHONY:	default all clean

default:	$(TARGET)
all:	default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c	$(HEADERS)
		$(CC)	$(CFLAGS) -c $< -o $@

.PRECIOUS:	$(TARGET) $(OBJECTS)

$(TARGET):	$(OBJECTS)
		$(CC)	$(OBJECTS) -Wall $(LIBS)
-o $@

clean:
		-rm -f *.o
		-rm -f $(TARGET)
