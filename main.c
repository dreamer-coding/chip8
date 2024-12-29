#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "chip8.h"

int main(int argc, char *argv[]){

    chip8_t chip8 = {0};
    chip8.rom = argv[1]; 
    chip8_init(&chip8);
    graphic_t window;
    init_sdl(&window,&chip8);

    bool running = true;

    SDL_Event sdl_event;
  
    while(chip8.state != QUIT){

        // Delay for approximately 60hz/60fps (16.67ms) or actual time elapsed

        handle_input(&chip8); 

        const uint64_t start_frame_time = SDL_GetPerformanceCounter();

        for(uint32_t i = 0 ; i < 600 / 60 ; i++){
            emulate_cycle(&chip8);
        }

        const uint64_t end_frame_time = SDL_GetPerformanceCounter();

        const double time_elapsed = (double)((end_frame_time - start_frame_time) * 1000) / SDL_GetPerformanceFrequency();
        SDL_Delay(16.67f > time_elapsed ? 16.67f - time_elapsed : 0);
        if(chip8.draw == true){
            update_screen(&window,&chip8);
            chip8.draw = false;
        }
        update_timers(window,&chip8);
    }


    destroy_sdl(&window);
    return 0;
}

