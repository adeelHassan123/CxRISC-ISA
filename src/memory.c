#include <stdio.h>
#include <string.h>
#include "isa.h"
#include "utils.h"

// byte-addressable array
static uint8_t dmem[DMEM_SIZE];

//set all bytes in data memory to 0
void mem_init(void) {
    memset(dmem, 0, sizeof(dmem));
}

int32_t dmem_read(uint32_t addr, bool is_byte) {
    //MMU logic later 
    if (addr >= DMEM_SIZE) return 0;

    //if True then load byte else load word
    if (is_byte) {
        return (int32_t)(int8_t)dmem[addr];  // Sign-extend byte
    } else {
        if (addr + 3 >= DMEM_SIZE) return 0;
        uint32_t word = dmem[addr] |
                        (dmem[addr+1] << 8) |
                        (dmem[addr+2] << 16) |
                        (dmem[addr+3] << 24);
        return (int32_t)word;
    }
}

void dmem_write(uint32_t addr, int32_t value, bool is_byte) {
    if (addr >= DMEM_SIZE) return;

    if (is_byte) {
        dmem[addr] = (uint8_t)value;
    } else {
        if (addr + 3 >= DMEM_SIZE) return;
        dmem[addr]     = value & 0xFF;
        dmem[addr + 1] = (value >> 8)  & 0xFF;
        dmem[addr + 2] = (value >> 16) & 0xFF;
        dmem[addr + 3] = (value >> 24) & 0xFF;
    }
}

void mem_dump(int start_byte, int end_byte) {
    if (start_byte < 0 || end_byte > DMEM_SIZE || start_byte >= end_byte) {
        printf("Invalid memory dump range\n");
        return;
    }
    
    char bin[33];
    for (int addr = start_byte; addr < end_byte; addr += 4) {
        if (addr + 3 >= DMEM_SIZE) break;
        uint32_t word = dmem[addr] |
                        (dmem[addr+1] << 8) |
                        (dmem[addr+2] << 16) |
                        (dmem[addr+3] << 24);
        int_to_binary(word, bin);
        printf("MEM[%3d] = %s  (%d)\n", addr, bin, (int32_t)word);
    }
}