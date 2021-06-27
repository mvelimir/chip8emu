#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "Chip8State.h"

Chip8State *InitChip8(void) {
    Chip8State *state = (Chip8State *)calloc(1, sizeof(Chip8State));

    state->memory = (uint8_t *)calloc(1024 * 4, 1);
    state->screen = &state->memory[0xF00];
    state->SP = 0xEA0;
    state->PC = 0x200;

    return state;
}

static void Op0(Chip8State *state, uint8_t *code) {
    
}

static void Op1(Chip8State *state, uint8_t *code) {
    uint16_t target = ((code[0] & 0x0f) << 8) | code[1];
    state->PC = target;
}

static void Op2(Chip8State *state, uint8_t *code) {
    
}

static void Op3(Chip8State *state, uint8_t *code) {
    uint8_t x = code[0] & 0x0f;
    if (state->V[x] == code[1])
        state->PC += 2;
    state->PC += 2;
}

static void Op4(Chip8State *state, uint8_t *code) {
    
}

static void Op5(Chip8State *state, uint8_t *code) {
    
}

static void Op6(Chip8State *state, uint8_t *code) {
    uint8_t x = code[0] & 0x0f;
    state->V[x] = code[1];
    state->PC += 2;
}

static void Op7(Chip8State *state, uint8_t *code) {
    
}

static void Op8(Chip8State *state, uint8_t *code) {
    
}

static void Op9(Chip8State *state, uint8_t *code) {
    
}

static void OpA(Chip8State *state, uint8_t *code) {
    state->I = ((op[0] & 0xf)<<8) | op[1];
    state->PC += 2;
}

static void OpB(Chip8State *state, uint8_t *code) {
    
}

static void OpC(Chip8State *state, uint8_t *code) {
    
}

static void OpD(Chip8State *state, uint8_t *code) {
    
}

static void OpE(Chip8State *state, uint8_t *code) {

}

static void OpF(Chip8State *state, uint8_t *code) {
    switch(code[1]) {
        case 0x7:
        case 0xa:
        case 0x15:
        case 0x18:
        case 0x1e:
        case 0x29:
        case 0x33: {
            uint8_t x = code[0]&0xf;    
            uint8_t ones, tens, hundreds;    
            uint8_t value = state->V[x];    
            ones = value % 10;    
            value = value / 10;    
            tens = value % 10;    
            hundreds = value / 10;    
            state->memory[state->I] = hundreds;    
            state->memory[state->I+1] = tens;    
            state->memory[state->I+2] = ones;
            break;
            }
            case 0x55:
            case 0x65:
        }
}

void EmulateChip8Op(Chip8State *state) {
    uint8_t *op = &state->memory[state->PC];
    uint8_t firstnib = (op[0] & 0xf0) >> 4;
    switch(firstnib) {
        case 0x0: Op1(state, op); break;
        case 0x1: Op1(state, op); break;
        case 0x2: Op1(state, op); break;
        case 0x3: Op3(state, op); break;
        case 0x4: Op4(state, op); break;
        case 0x5: Op5(state, op); break;
        case 0x6: Op6(state, op); break;
        case 0x7: Op7(state, op); break;
        case 0x8: Op8(state, op); break;
        case 0x9: Op9(state, op); break;
        case 0xa: OpA(state, op); break;
        case 0xb: OpB(state, op); break;
        case 0xc: OpC(state, op); break;
        case 0xd: OpD(state, op); break;
        case 0xe: OpE(state, op); break;
        case 0xf: OpF(state, op); break;
    }
}

static void UnimplementedInstruction(Chip8State *state) {
    printf("Error: Unimplemented instruction\n");
    DisassembleChip8Op(state->memory, state->PC);
    printf("\n");
    exit(1)
}

static void DisassembleChip8Op(uint8_t *codebuffer, int pc) {
  uint8_t *code = &codebuffer[pc];
  uint8_t firstnib = (code[0] >> 4);
  printf("%04x %02x %02x ", pc, code[0], code[1]);
  switch(firstnib) {
      case 0x0: {
            switch(code[1]) {
              case 0xe0: printf("CLS"); break;
              case 0xee: printf("RTS"); break;
              default: printf("UNKNOWN 0"); break;
            }
            break;
     }
      case 0x1: printf("%-10s $%01x%02x", "JUMP", code[0] & 0xf, code[1]); break;
      case 0x2: printf("%-10s $%01x%02x", "CALL", code[0] & 0xf, code[1]); break;
      case 0x3: printf("%-10s V%01X, #$%02x", "SKIP.EQ", code[0] & 0xf, code[1]); break;
      case 0x4: printf("%-10s V%01X, #$%02x", "SKIP.NE", code[0] & 0xf, code[1]); break;
      case 0x5: printf("%-10s V%01X, V%01X", "SKIP.EQ", code[0] & 0xf, code[1] & 0xf0); break;
      case 0x6: printf("%-10s V%01X, #$%02x", "MVI", code[0] & 0xf, code[1]); break;
      case 0x7: printf("%-10s V%01X, #$%02x", "ADI", code[0] & 0xf, code[1]); break;
      case 0x8: {
            switch(code[1] & 0xf) {
                case 0x0: printf("%-10s V%01X, V%01X", "MOV", code[0] & 0xf, code[1] & 0xf0); break;
                case 0x1: printf("%-10s V%01X, V%01X", "OR", code[0] & 0xf, code[1] & 0xf0); break;
                case 0x2: printf("%-10s V%01X, V%01X", "AND", code[0] & 0xf, code[1] & 0xf0); break;
                case 0x3: printf("%-10s V%01X, V%01X", "XOR", code[0] & 0xf, code[1] & 0xf0); break;
                case 0x4: printf("%-10s V%01X, V%01X", "ADD.", code[0] & 0xf, code[1] & 0xf0); break;
                case 0x5: printf("%-10s V%01X, V%01X", "SUB.", code[0] & 0xf, code[1] & 0xf0); break;
                case 0x6: printf("%-10s V%01X", "SHR.", code[0] & 0xf); break;
                case 0x7: printf("%-10s V%01X, V%01X", "SUBB.", code[0] & 0xf, code[1] & 0xf0); break;
                case 0xe: printf("%-10s V%01X", "SHL.", code[0] & 0xf); break;
                default: printf("UNKNOWN 8"); break;
            }
            break;
      }
      case 0x9: printf("%-10s V%01X, V%01X", "SKIP.EQ", code[0] & 0xf, code[1] & 0xf0); break;
      case 0xa: printf("%-10s I, #$%01x%02x", "MVI", code[0] & 0xf, code[1]); break;
      case 0xb: printf("%-10s $%01x%02x(V0)", "JUMP", code[0] & 0xf, code[1]); break;
      case 0xc: printf("%-10s V%01X, #$%02X", "RNDMSK", code[0] & 0xf, code[1]); break;
      case 0xd: printf("%-10s V%01X, V%01X, #$%01x", "SPRITE", code[0] & 0xf, code[1] & 0xf0, code[1] & 0xf); break;
      case 0xe: {
            switch(code[1]) {
                case 0x9e: printf("%-10s V%01X", "SKIP.KEY", code[0] & 0xf); break;
                case 0xa1: printf("%-10s V%01X", "SKIP.NOKEY", code[0] & 0xf); break;
                default: printf("UNKNOWN E"); break;
            }
            break;
      }
      case 0xf: {
            switch(code[1]) {
                case 0x7: printf("%-10s V%01X, DELAY", "MOV", code[0] & 0xf); break;
                case 0xa: printf("%-10s V%01X", "WAITKEY", code[0] & 0xf); break;
                case 0x15: printf("%-10s DELAY, V%01X", "MOV", code[0] & 0xf); break;
                case 0x18: printf("%-10s SOUND, V%01X", "MOV", code[0] & 0xf); break;
                case 0x1e: printf("%-10s I, V%01X", "ADD", code[0] & 0xf); break;
                case 0x29: printf("%-10s V%01X", "SPRITECHAR", code[0] & 0xf); break;
                case 0x33: printf("%-10s V%01X", "MOVBCD", code[0] & 0xf); break;
                case 0x55: printf("%-10s (I), V0-V%01X", "MOVM", code[0] & 0xf); break;
                case 0x65: printf("%-10s V0-V%01X, (I)", "MOVM", code[0] & 0xf); break;
                default: printf("UNKNOWN F"); break;
            }
            break;
      }
  }
}

int main(int argc, char **argv) {
    FILE *f = fopen(argv[1], "rb");
    if (f == NULL) {
        printf("Error: Couldn't open %s\n", argv[1]);
        exit(1);
    }

    fseek(f, 0L, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    unsigned char *buffer = (unsigned char *)malloc(fsize+0x200);    
    fread(buffer+0x200, fsize, 1, f);    
    fclose(f);

    int pc = 0x200;    
    while (pc < (fsize + 0x200)) {
        DisassembleChip8Op(buffer, pc);    
        pc += 2;    
        printf ("\n");    
    }      

    return 0;
}