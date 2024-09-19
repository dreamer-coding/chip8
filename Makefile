CC = gcc
SOURCES = main.c chip8.c
main: main.c
	$(CC) $(SOURCES) -o output
