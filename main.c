#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "chip8.h"

int main(int argc, char *argv[]){

   chip8_t chip8 = {0};
   chip8.rom = argv[1]; 
   chip8_init(&chip8);
   
   emulate_cycle(&chip8); 
   graphic_t window;
   init_sdl(&window);
   
   return 0;
}

