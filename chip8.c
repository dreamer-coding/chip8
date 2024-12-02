#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

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


uint16_t stack[16] = {0}; // 16 two-byte entries
uint16_t START_POINT = 0xF00;


unsigned char memory[4096] = {0}; // memory to hold 4096 bytes
const unsigned int START_ADDRESS = 0x200;

unsigned char *stream = &memory[START_ADDRESS];


bool display[64][32];
unsigned int pc = START_ADDRESS;
uint16_t *indexRegister;
uint8_t delay_timer = 60;
uint8_t sound_timer = 0;

uint8_t registers[NUM_REGISTERS] = {0};

// load the fontset to memory
void load_font_set(){
    unsigned char *startaddress = &memory[0x50];
    memcpy(startaddress,font,80);

}

 size_t bytesRead = 0;
// load the rom to memory
void loadRom(char const *filename){
    FILE *rom = fopen(filename, "rb");
    int size = 0;
    if(!rom){
        fprintf(stderr, "error reading rom from file\n");
    }
    else{
        fseek(rom, 0, SEEK_END);
        size = ftell(rom);
        fseek(rom, 0, SEEK_SET);

        unsigned char *buffer = malloc(sizeof(char)*size);
        bytesRead = fread(buffer,sizeof(unsigned char),size,rom);

        for(int i=0;i<size;i++){
            *stream++ = buffer[i];
        }

        //fread(&memory[START_ADDRESS],sizeof(unsigned char),size,rom);
    }
    fclose(rom);

};

// fetch

void fetch(){
    uint16_t opcode = memory[pc] << 8 | memory[pc+1]; // shift the program counter value by 8bits and OR operation to combine other value
                                                     
    printf("%x bit16 is \n",opcode);

    pc = pc + 2; // jump to two positions 

    printf("%x pc is \n",pc);

    unsigned short x = (opcode >> 8) & 0x000F; // the second nibble use to to look up one of the 16 registers (vx) from v0 through vf
    unsigned short y = (opcode >> 4) & 0x000F;
    printf("third nibble of %x is %x\n",opcode,y);
    unsigned short N = (opcode & 0x000F);
    printf("fourth nibble of %x is %x\n",opcode,N);
    unsigned short NN = (opcode & 0x00FF);

    unsigned short NNN = (opcode & 0x0FFF);

    printf("%x nibble x is \n",x);

    switch(opcode & 0xF000) {
    
            }
}

void printfetch(){
    for(int i = 0 ; i < bytesRead; i++){
        fetch();
        sleep(2);
    }
}

void printdata(){
    // check if rom loaded to memory
    for(int i=0x200 ; i < 0x300; i++){
        printf("address %d value %02x\n",i,memory[i]);
    }
    printf("loading fontset\n");
    load_font_set();
    for(int i = 0x50; i< 0x9F;i++)
    {
        printf("Address %x value %x\n",i, memory[i]);
    }
}
