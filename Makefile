CC = gcc
SOURCES = main.c chip8.c 
FLAGS = `sdl2-config --cflags --libs`
main: main.c
	$(CC) $(SOURCES) $(FLAGS) -g -o output
