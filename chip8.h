#ifndef CHIP8
#define CHIP8

typedef struct{
    uint16_t opcode;
    uint8_t X;
    uint8_t Y;
    uint8_t N;
    uint8_t NN;
    uint8_t NNN;
}instruction_t;


typedef struct{
    unsigned char ram[4096];
    uint16_t stack[16];
    bool display[64*32];
    uint16_t PC;
    uint16_t *IR;
    uint16_t registers[16];
    const char *rom;
    instruction_t inst;
}chip8_t;



void emulate_cycle(chip8_t *chip8);
void chip8_init(chip8_t *chip8);
#endif
