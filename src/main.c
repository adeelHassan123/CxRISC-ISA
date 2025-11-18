#include <stdio.h>
#include "isa.h"
#include "registers.h"
#include "memory.h"
#include "program_loader.h"
#include "single_cycle.h"

#define PROGRAM_PATH "program/prog.asm"

int main() {
    uint32_t imem[IMEM_SIZE] = {0};
    int prog_size = 0;

    printf("RISC-V Single-Cycle CPU Simulator \n");
    regs_init();
    mem_init();

    if (load_program(PROGRAM_PATH, imem, &prog_size) != 0) {
        printf("Failed to load program\n", PROGRAM_PATH);
        return 1;
    }

    printf("Starting execution... Full trace in 'trace.txt'\n\n");

    run_program(imem, prog_size);

    printf("\nExecution complete! Check 'trace.txt' for detailes\n");

    return 0;
}