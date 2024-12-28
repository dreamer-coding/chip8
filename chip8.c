#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "chip8.h"

const uint8_t font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void init_sdl(graphic_t *sdl) {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    printf("SDL could not be initalized! SDL_ERROR: %s\n", SDL_GetError());
  } else {

    sdl->window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN);
    if (sdl->window == NULL) {
      printf("Window could not be created: %s\n", SDL_GetError());

    } else {

      sdl->renderer =
          SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
      if (!sdl->renderer) {
        printf("Could not create renderer:%s\n", SDL_GetError());
      }
    }

     }

    SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdl->renderer);

    SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 255);

}


void update_screen(graphic_t *sdl,chip8_t *chip8){


    for(int y=0; y < 32; y++){
        for(int x = 0; x < 64; x++){
            if(chip8->display[x][y] == 1){
                SDL_Rect r = {
                    x * 10,
                    y * 10,
                    10,
                    10,
                };

                SDL_RenderFillRect(sdl->renderer, &r);
            }

        }
    }


    SDL_RenderPresent(sdl->renderer);
}

void handle_input(chip8_t *chip8){
        
    SDL_Event event;

    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                chip8->state = QUIT;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        chip8->state = QUIT;
                        break;
                    case SDLK_1:chip8->keypad[0x1] = 1; break;
                    case SDLK_2:chip8->keypad[0x2] = 1; break;
                    case SDLK_3:chip8->keypad[0x3] = 1; break;
                    case SDLK_4:chip8->keypad[0xc] = 1; break;

                    case SDLK_q:chip8->keypad[0x4] = 1; break;
                    case SDLK_w:chip8->keypad[0x5] = 1; break;
                    case SDLK_e:chip8->keypad[0x6] = 1; break;
                    case SDLK_r:chip8->keypad[0xd] = 1; break;

                    case SDLK_a:chip8->keypad[0x7] = 1; break;
                    case SDLK_s:chip8->keypad[0x8] = 1; break;
                    case SDLK_d:chip8->keypad[0x9] = 1; break;
                    case SDLK_f:chip8->keypad[0xe] = 1; break;
                                
                    case SDLK_z:chip8->keypad[0xa] = 1; break;
                    case SDLK_x:chip8->keypad[0x0] = 1; break;
                    case SDLK_c:chip8->keypad[0xb] = 1; break;
                    case SDLK_v:chip8->keypad[0xf] = 1; break;



                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        chip8->state = QUIT;
                        break;
                    case SDLK_1:chip8->keypad[0x1] = 0; break;
                    case SDLK_2:chip8->keypad[0x2] = 0; break;
                    case SDLK_3:chip8->keypad[0x3] = 0; break;
                    case SDLK_4:chip8->keypad[0xc] = 0; break;

                    case SDLK_q:chip8->keypad[0x4] = 0; break;
                    case SDLK_w:chip8->keypad[0x5] = 0; break;
                    case SDLK_e:chip8->keypad[0x6] = 0; break;
                    case SDLK_r:chip8->keypad[0xd] = 0; break;

                    case SDLK_a:chip8->keypad[0x7] = 0; break;
                    case SDLK_s:chip8->keypad[0x8] = 0; break;
                    case SDLK_d:chip8->keypad[0x9] = 0; break;
                    case SDLK_f:chip8->keypad[0xe] = 0; break;

                    case SDLK_z:chip8->keypad[0xa] = 0; break;
                    case SDLK_x:chip8->keypad[0x0] = 0; break;
                    case SDLK_c:chip8->keypad[0xb] = 0; break;
                    case SDLK_v:chip8->keypad[0xf] = 0; break;
                    
                    default:
                                break;
                }
                break;

            default:
                break;
        }
    }

}

int destroy_sdl(graphic_t * window) {
    SDL_DestroyWindow(window->window);
    SDL_Quit();
    return 0;
}

void delay_timer(){
    static Uint64 last_time = 0;
    Uint64 current_time = SDL_GetTicks();
    Uint64 frame_time = 1000 / 60;

    if(current_time - last_time < frame_time){
        SDL_Delay(frame_time - (current_time - last_time));
    }
    last_time = SDL_GetTicks();

}

void update_timers(chip8_t *chip8){
    if(chip8->delay_timer > 0) 
        chip8->delay_timer--;
    if(chip8->sound_timer > 0){
    }
        
}
// Init chip8 data
 void chip8_init(chip8_t * chip8) {
    FILE *rom = fopen(chip8->rom, "rb"); // load the rom
    uint16_t entry_point = 0x200;
    chip8->stack_ptr = chip8->stack;
    chip8->PC = entry_point;
    chip8->state = RUNNING;

    if (!rom) {
      fprintf(stdout, "Error Openning rom or rom file not exists %s\n",
              chip8->rom);
    }

    fseek(rom, 0, SEEK_END);
    long fsize = ftell(rom);
    rewind(rom);

    if (fread(&chip8->ram[entry_point], fsize, 1, rom) != 0) {
      fprintf(stdout, "rom loaded\n");
    }
    fclose(rom);

    memcpy(&chip8->ram[0x50], font, 0x09F - 0x050); // load the fontset
}

void debug_info(chip8_t *chip8){

    printf("opcode %x\n",chip8->inst.opcode);
    printf("Current PC: %4x\n", chip8->PC);

}


  // Emulate the chip8 cycle
void emulate_cycle(chip8_t * chip8) {

    chip8->inst.opcode =
        chip8->ram[chip8->PC] << 8 |
        chip8->ram[chip8->PC + 1]; // shift the program counter value by 8bits
                                   // and OR operation to combine other value

    chip8->PC +=2;

    chip8->inst.X = (chip8->inst.opcode >> 8) & 0x000F;
    chip8->inst.Y = (chip8->inst.opcode >> 4) & 0x000F;
    chip8->inst.N = (chip8->inst.opcode & 0x000F);
    chip8->inst.NN = (chip8->inst.opcode & 0x00FF);
    chip8->inst.NNN = (chip8->inst.opcode & 0x0FFF);

    debug_info(chip8);

    switch ((chip8->inst.opcode >> 12) & 0x0F) {
        case 0x00:
            switch (chip8->inst.opcode & 0x0FFF) {
                case 0xEE:
                    chip8->PC = *--chip8->stack_ptr;
                    break;
                case 0xE0:
                    memset(chip8->display, false, sizeof chip8->display);
                    chip8->draw = true;
                    break;
            }
            break;

        case 0x01:
            chip8->PC = chip8->inst.NNN;
            break;
        case 0x02:
                *chip8->stack_ptr++ = chip8->PC;
                chip8->PC = chip8->inst.NNN;
                break;
        case 0x03:
            if (chip8->V[chip8->inst.X] == chip8->inst.NN) {
                chip8->PC += 2;
            }
            break;
        case 0x04:
            if (chip8->V[chip8->inst.X] != chip8->inst.NN) {
                chip8->PC += 2;
            }
            break;
        case 0x05:
            if (chip8->V[chip8->inst.X] == chip8->inst.Y) {
                chip8->PC += 2;
            }
            break;
        case 0x06:
            chip8->V[chip8->inst.X] = chip8->inst.NN;
            break;
        case 0x07:
            printf("going for case 0x7000\n");
            chip8->V[chip8->inst.X] += chip8->inst.NN;
            break;
        case 0x08:
            printf("going for case 0x8000\n");
            switch (chip8->inst.N) {
                case 0:
                    chip8->V[chip8->inst.X] = chip8->V[chip8->inst.Y];
                    break;
                case 1:
                    chip8->V[chip8->inst.X] =
                        (chip8->V[chip8->inst.X] | chip8->V[chip8->inst.Y]);
                    break;
                case 2:
                    chip8->V[chip8->inst.X] =
                        (chip8->V[chip8->inst.X] & chip8->V[chip8->inst.Y]);
                    break;
                case 3:
                    chip8->V[chip8->inst.X] =
                        (chip8->V[chip8->inst.X] ^ chip8->V[chip8->inst.Y]);
                    break;
                case 4:
                    chip8->carry_flag = (uint16_t)((chip8->V[chip8->inst.X] +
                                chip8->V[chip8->inst.Y]) > 255);
                    chip8->V[chip8->inst.X] =
                        (chip8->V[chip8->inst.X] + chip8->V[chip8->inst.Y]) & 0x00FF;
                    chip8->V[0xF] = chip8->carry_flag;
                    break;
                case 5:
                    chip8->carry_flag =
                        (uint16_t)(chip8->V[chip8->inst.X] >= chip8->V[chip8->inst.Y]);
                    chip8->V[chip8->inst.X] -= chip8->V[chip8->inst.Y];
                    chip8->V[0xF] = chip8->carry_flag;
                    break;
                case 6:
                    chip8->carry_flag = chip8->V[chip8->inst.X] & 1;
                    chip8->V[chip8->inst.X] >>=1;
                    chip8->V[0xF] = chip8->carry_flag;
                    break;
                case 7:
                    chip8->carry_flag =(uint16_t)(chip8->V[chip8->inst.Y] >= chip8->V[chip8->inst.X]);
                    chip8->V[chip8->inst.X] = (chip8->V[chip8->inst.Y] - chip8->V[chip8->inst.X]);
                    chip8->V[0xF] = chip8->carry_flag;
                    break;
                case 0xE:
                    chip8->carry_flag = chip8->V[chip8->inst.X] >> 7; 
                    chip8->V[chip8->inst.X] <<= 1;
                    chip8->V[0xF] = chip8->carry_flag;
                    break;
            }
            break;
        case 0x09:
            if (chip8->V[chip8->inst.X] != chip8->V[chip8->inst.Y]) {
                chip8->PC += 2;
            }
            break;
        case 0x0A:
            chip8->I = chip8->inst.NNN;
            break;
        case 0x0B:
            chip8->PC = chip8->inst.NNN + chip8->V[0x0];
            break;
        case 0x0C:
            chip8->V[chip8->inst.X] = (rand() % 255 + 0) & chip8->inst.NN;
            break;
        case 0x0D:
            uint8_t x = chip8->V[chip8->inst.X] % 64;
            uint8_t y = chip8->V[chip8->inst.Y] % 32;
            uint8_t height = chip8->inst.N;
            uint8_t pixel;

            chip8->V[0xF] = 0;

            for (int row = 0; row < height; row++) {
                if((chip8->ram[chip8->I + row] ) < sizeof(chip8->ram)){
                pixel = chip8->ram[chip8->I + row];
                }
                else{
                    break;
                }

                for (int col = 0; col < 8; col++) {
                    if ((pixel & (0x80 >> col)) != 0) {
                        int index = (x + col) + ((y + row) * 64);

                        if (chip8->display[x + col][y + row] == 1) {
                            chip8->V[0xF] = 1;
                        }

                        chip8->display[x + col][y + row] ^= 1;
                    }
                }
            }
            chip8->draw = true;
            break;
        case 0x0E:
            if (chip8->inst.NN == 0x9E) {
                if (chip8->keypad[chip8->V[chip8->inst.X]]) {
                    chip8->PC += 2;
                }
            } else if (chip8->inst.NN == 0xA1) {
                if (!chip8->keypad[chip8->V[chip8->inst.X]]) {
                    chip8->PC += 2;
                }
            }
            break;
        case 0x0F:
            static bool key_pressed = false;
            switch (chip8->inst.NN) {
                case 0x07:
                    chip8->V[chip8->inst.X] = chip8->delay_timer;
                    break;
                case 0x0A:
                    for (int i = 0; i < sizeof chip8->keypad; i++) {
                        if (chip8->keypad[i]) {
                            key_pressed = true;
                            chip8->V[chip8->inst.X] = i;
                            break;
                        }
                    }
                    if (!key_pressed) {
                        chip8->PC -= 2;
                    }
                    break;
                case 0x15:
                    chip8->delay_timer = chip8->V[chip8->inst.X];
                    break;
                case 0x18:
                    chip8->sound_timer = chip8->V[chip8->inst.X];
                    break;
                case 0x1E:
                    chip8->I += chip8->V[chip8->inst.X];
                    break;
                case 0x29:
                    chip8->I = chip8->V[chip8->inst.X] * 5;
                    break;
                case 0x33:
                    chip8->ram[chip8->I + 2] = chip8->V[chip8->inst.X] % 10;
                    chip8->ram[chip8->I + 1] = (chip8->V[chip8->inst.X] / 10 ) % 10;
                    chip8->ram[chip8->I] = chip8->V[chip8->inst.X] / 100; 
                    break;
                case 0x55:
                    for (uint8_t i = 0; i <= chip8->inst.X; i++) {
                        chip8->ram[chip8->I++] = chip8->V[i];
                    }

                    break;

                case 0x65:
                    for (uint8_t i = 0; i <= chip8->inst.X; i++) {
                        chip8->V[i] = chip8->ram[chip8->I++];
                    }

                    break;
            }
        default:
            break;
    }
}
