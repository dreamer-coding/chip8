#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "chip8.h"

int main(int argc, char *argv[]){

    chip8_t chip8 = {0};
    chip8.rom = argv[1]; 
    chip8_init(&chip8);
    graphic_t window;
    init_sdl(&window);

    bool running = true;

    SDL_Event sdl_event;
  
    while(chip8.state != QUIT){

            handle_input(&chip8); 

            emulate_cycle(&chip8);
            if(chip8.draw == true){
                update_screen(&window,&chip8);
                chip8.draw = false;
            }
    }
    

    destroy_sdl(&window);
    return 0;
}

