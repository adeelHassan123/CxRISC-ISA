#include <stdio.h>
#include "isa.h"
#include "utils.h"

void regs_init();
void regs_dump();
void mem_init();
void mem_dump(int start, int end);
int load_program(const char *path, Instr prog[], int *count);
void run_program(Instr prog[], int prog_size);
void mem_write(int addr, int val); 

#define PROGRAM_PATH "program/prog.asm"

int main() {
    Instr program[NUM_INST];  
    int prog_count = 0;

    // Step 1: Initialize registers and memory 
    regs_init();
    mem_init();

    // Step 2: Preload memory for testing
    mem_write(0, 5);
    mem_write(1, 3);

    // Step 3: Load program
    if (load_program(PROGRAM_PATH, program, &prog_count) != 0) {
        printf("Error: could not load program at '%s'\n", PROGRAM_PATH);
        return 1;
    }

    printf("Program loaded: %d instructions\n", prog_count);

    // Step 4: Run program
    run_program(program, prog_count);

    // Step 5: Dump final state
    regs_dump();         
    mem_dump(0, 10);   

    return 0;
}