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

    SDL_Event chip8_event;
    
    while(running){
        while(SDL_PollEvent(&chip8_event)){

            if(chip8_event.type == SDL_QUIT){
                running = false;
            }
            
            emulate_cycle(&window,&chip8); 
            if(chip8.draw == true){
                printf("printing\n");
                update_screen(&window,&chip8);
                chip8.draw = false;
            }
            else{
                printf("not working\n");
            }

           // delay_timer();
        }

    }



//    destroy_sdl(&window);
    return 0;
}

