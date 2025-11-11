#include "isa.h"
#include "registers.h"
#include "memory.h"
#include <stdio.h>

void run_program(Instr prog[], int prog_size) {
    int pc = 0; 

    while (pc >= 0 && pc < prog_size) {
        Instr *I = &prog[pc];

        switch (I->op) {

            // Arithmetic / Logical 
            case OP_ADD:
                regs_write(I->rd, regs_read(I->rs1) + regs_read(I->rs2));
                break;
            case OP_ADDI:
                regs_write(I->rd, regs_read(I->rs1) + I->imm);
                break;

            // Load / Store
            case OP_LB: case OP_LW:
                regs_write(I->rd, mem_read(I->imm));
                break;
            case OP_SB: case OP_SW:
                mem_write(I->imm, regs_read(I->rs1));
                break;

            // Control 
            case OP_NOP:
            case OP_UNKNOWN:
                break;
        }
        pc++; // next instruction
    }
}
