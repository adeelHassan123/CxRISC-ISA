#include <stdio.h>
#include "isa.h"
#include "utils.h"

static int mem[MEM_SIZE];

// Init all Memory blocks to zero
void mem_init() {
    for (int i = 0; i < MEM_SIZE; i++)
        mem[i] = 0;
}

// Read data from Memory
int mem_read(int addr) {
    if (addr < 0 || addr >= MEM_SIZE)
        return 0;
    return mem[addr];
}

// Write data to memory
void mem_write(int addr, int value) {
    if (addr < 0 || addr >= MEM_SIZE)
        return;
    mem[addr] = value;
}

// Print first few memory values
void mem_dump(int strt_addr , int end_addr) {
    if(strt_addr >= 0 && strt_addr < end_addr && end_addr <= MEM_SIZE) {
        for (int i = strt_addr; i < end_addr; i++) {
            printf("[%d] = %d\n", i, mem[i]);
        }
    } else {
        printf("Invalid memory range: %d to %d\n", strt_addr, end_addr);
    }
}
