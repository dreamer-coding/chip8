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

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not be initalized! SDL_ERROR: %s\n", SDL_GetError());
  } else {

    sdl->window = SDL_CreateWindow("CHIP8", 0, 0, 640, 320, 0);
    if (sdl->window == NULL) {
      printf("Window could not be created: %s\n", SDL_GetError());

    } else {

      sdl->renderer =
          SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
      if (!sdl->renderer) {
        printf("Could not create renderer:%s\n", SDL_GetError());
      }
    }

    SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 0);
    SDL_RenderClear(sdl->renderer);

    SDL_Rect r;
    r.x = 50;
    r.y = 50;
    r.h = 150;
    r.w = 150;

    SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 255);

    SDL_RenderFillRect(sdl->renderer, &r);

    SDL_RenderPresent(sdl->renderer);

    SDL_Delay(500);
  }
}

  int destroy_sdl(graphic_t * window) {
    SDL_DestroyWindow(window->window);
    SDL_Quit();
    return 0;
  }

  // Init chip8 data
 void chip8_init(chip8_t * chip8) {
    FILE *rom = fopen(chip8->rom, "rb"); // load the rom
    uint16_t entry_point = 0x200;

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

  // Emulate the chip8 cycle
void emulate_cycle(chip8_t * chip8) {

    chip8->inst.opcode =
        chip8->ram[chip8->PC] << 8 |
        chip8->ram[chip8->PC + 1]; // shift the program counter value by 8bits
                                   // and OR operation to combine other value

    chip8->PC = chip8->PC + 2;

    chip8->inst.X = (chip8->inst.opcode >> 8) & 0x000F;
    chip8->inst.Y = (chip8->inst.opcode >> 4) & 0x000F;
    chip8->inst.N = (chip8->inst.opcode & 0x000F);
    chip8->inst.NN = (chip8->inst.opcode & 0x00FF);
    chip8->inst.NNN = (chip8->inst.opcode & 0x0FFF);

    switch (chip8->inst.opcode & 0xF000) {
    default:
      break;
    case 0x0000:
      switch (chip8->inst.opcode & 0x00FF) {
      case 0xEE:
        chip8->PC = *(chip8->stack_ptr - 1);
        break;
      case 0xE0:
        memset(chip8->display, false, sizeof chip8->display);
        break;
      }
      break;

    case 0x1000:
      chip8->PC = chip8->inst.NNN;
      break;
    case 0x2000:
      *(++chip8->stack_ptr) = chip8->PC;
      chip8->PC = chip8->inst.NNN;
      break;
    case 0x3000:
      if (chip8->V[chip8->inst.X] == chip8->inst.NN) {
        chip8->PC += 2;
      }
      break;
    case 0x4000:
      if (chip8->V[chip8->inst.X] != chip8->inst.NN) {
        chip8->PC += 2;
      }
      break;
    case 0x5000:
      if (chip8->V[chip8->inst.X] == chip8->inst.Y) {
        chip8->PC += 2;
      }
      break;
    case 0x6000:
      chip8->V[chip8->inst.X] = chip8->inst.NN;
      break;
    case 0x7000:
      chip8->V[chip8->inst.X] += chip8->inst.NN;
      break;
    case 0x8000:
      switch (chip8->inst.opcode & 0x000F) {
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
            (uint16_t)(chip8->V[chip8->inst.X] > chip8->V[chip8->inst.Y]);
        chip8->V[chip8->inst.X] -= chip8->V[chip8->inst.Y];
        chip8->V[0xF] = chip8->carry_flag;
        break;
      case 6:
        chip8->V[0xF] = chip8->V[chip8->inst.X] & 1;
        chip8->V[chip8->inst.X] >>= 1;
        break;
      case 7:
        chip8->V[chip8->inst.X] =
            chip8->V[chip8->inst.Y] - chip8->V[chip8->inst.X];
        chip8->carry_flag =
            (uint16_t)(chip8->V[chip8->inst.Y] >= chip8->V[chip8->inst.X]);
        chip8->V[0xF] = chip8->carry_flag;
        break;
      case 0xE:
        chip8->V[0xF] = chip8->V[chip8->inst.X] >> 7;
        chip8->V[chip8->inst.X] <<= 1;
        break;
      }
      break;
    case 0x9000:
      if (chip8->V[chip8->inst.X] != chip8->V[chip8->inst.Y]) {
        chip8->PC += 2;
      }
      break;
    case 0xA000:
      chip8->I = chip8->inst.NNN;
      break;
    case 0xB000:
      chip8->PC = chip8->inst.NNN + chip8->V[0x0];
      break;
    case 0xC000:
      chip8->V[chip8->inst.X] = (rand() % 255 + 0) & chip8->inst.NN;
      break;
    case 0xD000:
      uint8_t x = chip8->V[chip8->inst.X] % 64;
      uint8_t y = chip8->V[chip8->inst.Y] % 32;
      uint8_t height = chip8->inst.N;
      uint8_t pixel;

      chip8->V[0xF] = 0;

      for (int row = 0; row < height; row++) {
        pixel = chip8->ram[chip8->I + row];

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
      break;
    case 0xE000:
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
    case 0xF000:
      static bool key_pressed = false;
      switch (chip8->inst.NN) {
      case 0x07:
        chip8->V[chip8->inst.X] = chip8->dt;
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
        chip8->dt = chip8->V[chip8->inst.X];
        break;
      case 0x18:
        chip8->st = chip8->V[chip8->inst.X];
        break;
      case 0x1E:
        chip8->I += chip8->V[chip8->inst.X];
        break;
      case 0x29:
        chip8->I += chip8->V[chip8->inst.X] * 5;
        break;
      case 0x33:
        uint16_t bcd_value = chip8->V[chip8->inst.X];
        uint16_t bcd = 0;
        int shift = 0;

        while (bcd_value > 0) {
          bcd |= (bcd_value % 10) << (shift++ << 2);
          bcd /= 10;
        }

        chip8->ram[chip8->I + 2] = bcd % 10;
        bcd /= 10;
        chip8->ram[chip8->I + 1] = bcd % 10;
        bcd /= 10;
        chip8->ram[chip8->I] = bcd;

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
    }
}

