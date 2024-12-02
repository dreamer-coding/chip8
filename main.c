#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "chip8.h"

int main(int argc, char *argv[]){
    
    loadRom(argv[1]);
    printdata();
    printfetch();
    return 0;
}

