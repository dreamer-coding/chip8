CC = gcc
SOURCES = main.c chip8.c 
FLAGS = `sdl2-config --cflags --libs` 
OUTPUT = output
main: main.c
	$(CC) $(SOURCES) $(FLAGS) -Werror -g -o $(OUTPUT) 
