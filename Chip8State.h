#include <stdint.h>

typedef struct {
    uint8_t V[16];
    uint16_t I;
    uint16_t SP;
    uint16_t PC;
    uint8_t delay;
    uint8_t sound;
    uint8_t *memory;
    uint8_t *screen;
} Chip8State;

Chip8State* InitChip8(void);

void EmulateChip8Op(Chip8State *state);