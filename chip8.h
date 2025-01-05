/**
 * @file chip8.h
 * @brief CHIP-8 emulator header file with SDL2 support.
 * 
 * This file defines the structures, enumerations, and functions required for the
 * implementation of a CHIP-8 emulator, including SDL2-based graphics and audio handling.
 */

#ifndef CHIP8
#define CHIP8

#include <SDL2/SDL.h>
#include <stdbool.h>

/**
 * @enum chip8_state_t
 * @brief Enumeration for the current state of the CHIP-8 emulator.
 */
typedef enum {
    QUIT,      /**< Indicates the emulator should quit. */
    RUNNING    /**< Indicates the emulator is currently running. */
} chip8_state_t;

/**
 * @struct instruction_t
 * @brief Represents a decoded CHIP-8 instruction.
 */
typedef struct {
    uint16_t opcode; /**< Full 16-bit opcode. */
    uint8_t X;       /**< X register (4 bits). */
    uint8_t Y;       /**< Y register (4 bits). */
    uint8_t N;       /**< Last nibble (4 bits). */
    uint8_t NN;      /**< 8-bit immediate value. */
    uint16_t NNN;    /**< 12-bit immediate address. */
} instruction_t;

/**
 * @struct chip8_t
 * @brief Represents the state of the CHIP-8 emulator.
 */
typedef struct {
    uint8_t ram[4096];       /**< Main memory (4 KB). */
    uint16_t stack[16];      /**< Call stack. */
    uint16_t *stack_ptr;     /**< Stack pointer. */
    bool display[64][32];    /**< Monochrome display (64x32 pixels). */
    uint8_t V[16];           /**< General-purpose registers (V0-VF). */
    uint16_t PC;             /**< Program counter. */
    uint16_t I;              /**< Index register. */
    uint16_t registers[16];  /**< Extended registers (not standard). */
    uint16_t keypad[16];     /**< Keypad state (16 keys). */
    const char *rom;         /**< Loaded ROM file path. */
    uint16_t carry_flag;     /**< Carry flag for arithmetic operations. */
    uint8_t last_key;        /**< Last pressed key. */
    uint8_t pixel_scale;     /**< Scale factor for pixel rendering. */
    bool draw;               /**< Flag indicating if the screen should be redrawn. */
    uint8_t delay_timer;     /**< Delay timer. */
    uint8_t sound_timer;     /**< Sound timer. */
    chip8_state_t state;     /**< Current emulator state. */
    instruction_t inst;      /**< Current instruction being executed. */
} chip8_t;

/**
 * @struct graphic_t
 * @brief Represents the SDL2 graphics and audio context.
 */
typedef struct {
    SDL_Window *window;       /**< SDL2 window handle. */
    SDL_Renderer *renderer;   /**< SDL2 renderer handle. */
    SDL_AudioSpec want;       /**< Desired SDL2 audio specification. */
    SDL_AudioSpec have;       /**< Actual SDL2 audio specification. */
    SDL_AudioDeviceID dev;    /**< SDL2 audio device ID. */
} graphic_t;

/**
 * @brief Initializes the CHIP-8 emulator state.
 * 
 * @param chip8 Pointer to the CHIP-8 state structure.
 */
void chip8_init(chip8_t *chip8);

/**
 * @brief Executes a single emulation cycle of the CHIP-8 emulator.
 * 
 * @param chip8 Pointer to the CHIP-8 state structure.
 */
void emulate_cycle(chip8_t *chip8);

/**
 * @brief Initializes SDL2 and prepares graphics and audio.
 * 
 * @param sdl Pointer to the SDL2 context structure.
 * @param chip8 Pointer to the CHIP-8 state structure.
 */
void init_sdl(graphic_t *sdl, chip8_t *chip8);

/**
 * @brief Clears the SDL2 screen.
 * 
 * @param sdl Pointer to the SDL2 context structure.
 */
void clear_screen(graphic_t *sdl);

/**
 * @brief Destroys the SDL2 context and releases resources.
 * 
 * @param sdl Pointer to the SDL2 context structure.
 * @return int Return 0 on success, non-zero on failure.
 */
int destroy_sdl(graphic_t *sdl);

/**
 * @brief Updates the SDL2 screen based on the CHIP-8 display state.
 * 
 * @param sdl Pointer to the SDL2 context structure.
 * @param chip8 Pointer to the CHIP-8 state structure.
 */
void update_screen(graphic_t *sdl, chip8_t *chip8);

/**
 * @brief Simulates a delay timer.
 */
void delay_timer();

/**
 * @brief Handles user input from the keyboard and updates the CHIP-8 state.
 * 
 * @param chip8 Pointer to the CHIP-8 state structure.
 */
void handle_input(chip8_t *chip8);

/**
 * @brief SDL2 audio callback function for generating sound.
 * 
 * @param userdata Pointer to user-defined data.
 * @param stream Pointer to the audio stream buffer.
 * @param len Length of the audio buffer.
 */
void audio_callback(void *userdata, Uint8 *stream, int len);

/**
 * @brief Updates the delay and sound timers for the CHIP-8 emulator.
 * 
 * @param sdl SDL2 context structure.
 * @param chip8 Pointer to the CHIP-8 state structure.
 */
void update_timers(graphic_t sdl, chip8_t *chip8);

#endif /* CHIP8 */
