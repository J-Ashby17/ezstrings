CC=gcc
CFLAGS=-O2 -Wall -Wextra -std=c11

OBJS=main.o parser.o search.o

ezstrings: $(OBJS)
	$(CC) $(CFLAGS) -o ezstrings $(OBJS)

main.o: main.c parser.h search.h
parser.o: parser.c parser.h
search.o: search.c search.h

clean:
	rm -f *.o ezstrings *.exe
