#ifndef CHIP8
#define CHIP8

#include <SDL2/SDL.h>
#include <stdbool.h>



typedef struct{
    uint16_t opcode;
    uint8_t X;
    uint8_t Y;
    uint8_t N;
    uint8_t NN;
    uint8_t NNN;
}instruction_t;

typedef struct{
    uint8_t ram[4096];
    uint16_t stack[16];
    uint16_t *stack_ptr;
    bool display[64][32];
    uint8_t V[16];
    uint16_t PC;
    uint16_t I;
    uint16_t registers[16];
    uint16_t keypad[16];
    const char *rom;
    unsigned char dt;
    unsigned char st;
    uint16_t carry_flag;
    instruction_t inst;
}chip8_t;

typedef struct{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect  *rect;
}graphic_t;



void chip8_init(chip8_t *chip8);
void emulate_cycle(chip8_t *chip8);
//void emulate_cycle(chip8_t *chip8);
//void chip8_init(chip8_t *chip8);
void init_sdl(graphic_t *sdl);
//int destroy_sdl(graphic_t *sdl);


#endif
