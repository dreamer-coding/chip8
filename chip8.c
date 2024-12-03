#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "chip8.h"

#define NUM_REGISTERS 16

#define V0 registers[0]
#define V1 registers[1]
#define V2 registers[2]
#define V3 registers[3]
#define V4 registers[4]
#define V5 registers[5]
#define V6 registers[6]
#define V7 registers[7]
#define V8 registers[8]
#define V9 registers[9]
#define VA registers[10]
#define VB registers[11]
#define VC registers[12]
#define VD registers[13]
#define VE registers[14]
#define VF registers[15]


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


// Init chip8 data
void chip8_init(chip8_t *chip8){
    

}

// Emulate the chip8 cycle
void emulate_cycle(chip8_t *chip8){
    
    chip8->inst.opcode = chip8->ram[chip8->PC] << 8 | chip8->ram[chip8->PC+1]; // shift the program counter value by 8bits and OR operation to combine other value
    
    chip8->PC = chip8->PC+ 2; 
    
    chip8->inst.X = (chip8->inst.opcode >> 8) & 0x000F; 
    chip8->inst.Y = (chip8->inst.opcode >> 4) & 0x000F;
    chip8->inst.N = (chip8->inst.opcode & 0x000F);
    chip8->inst.NN = (chip8->inst.opcode & 0x00FF); 
    chip8->inst.NNN = (chip8->inst.opcode & 0x0FFF);

    switch(chip8->inst.opcode & 0xF000) {
        default:
            break;
        case 0x0000:
            switch(chip8->inst.opcode & 0x00FF){
                case 0xE0:
                    break;
                case 0xEE:
                    break;
            }
    
            }
}





