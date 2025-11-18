#include <stdio.h>
#include "isa.h"
#include "registers.h"
#include "memory.h"
#include "decode.h"
#include "execute.h"
#include "trace.h"

#define PROGRAM_PATH "program/prog.asm"

void run_program(uint32_t imem[], int prog_size_words) {
    trace_init(PROGRAM_PATH, prog_size_words);

    uint32_t pc = 0;
    const uint32_t end_pc = prog_size_words * 4;
    int cycle = 1;

    while (pc < end_pc) {
        // FETCH
        uint32_t raw_instr = imem[pc >> 2];

        // DECODE 
        DecodedInstr ins = decode(raw_instr);

        if (ins.op == OP_UNKNOWN) {
            fprintf(trace_file, "ERROR: Unknown instruction at PC=0x%04x\n", pc);
            break;
        }

        // REGISTER READ
        int32_t rs1_val = (ins.rs1 >= 0) ? regs_read(ins.rs1) : 0;
        int32_t rs2_val = (ins.rs2 >= 0) ? regs_read(ins.rs2) : 0;

        // EXECUTE + MEMORY 
        ExecuteResult ex = execute_instruction(ins, rs1_val, rs2_val);

        //  WRITEBACK 
        execute_writeback(ins, ex.wb_data);

        //  TRACE OUTPUT
        trace_cycle(pc, raw_instr, ins, rs1_val, rs2_val,
                    ex.alu_result, ex.mem_data, ex.wb_data,
                    cycle++, ex.mem_read, ex.mem_write);

        pc += 4;
    }

    trace_final(cycle - 1);
    trace_close();
}