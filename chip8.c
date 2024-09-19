#include <stdio.h>
#include <stdlib.h>

void loadRom(char const *filename,unsigned char *stream){
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
        size_t bytesRead = fread(buffer,sizeof(unsigned char),size,rom);
        
        *stream = *stream++;
        for(int i=0;i<size;i++){
            *stream++ = buffer[i];
        }
        
    }
    fclose(rom);
};


