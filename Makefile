CC = gcc
SOURCES = main.c chip8.c
main: main.c
	$(CC) $(SOURCES) -g -o output
