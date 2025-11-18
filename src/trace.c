#include <stdio.h>
#include <time.h>
#include <string.h>
#include "trace.h"
#include "registers.h"
#include "memory.h"
#include "utils.h"

void regs_dump_detailed(FILE *f) {
    fprintf(f, "--- Current Register File ---\n");
    char bin[33];
    for (int i = 0; i < NUM_REGS; i++) {
        int32_t val = regs_read(i);
        int_to_binary(val, bin);
        fprintf(f, "R%d = %s (%d)\n", i, bin, val);
    }
    fprintf(f, "\n");
}

void trace_init(const char *program_path, int prog_size_words) {
    trace_file = fopen("trace.txt", "w");
    if (!trace_file) trace_file = stdout;

    time_t now = time(NULL);
    fprintf(trace_file, "=== RISC-V SINGLE-CYCLE CPU - DETAILED TRACE ===\n");
    fprintf(trace_file, "Program: %s | Instructions: %d\n\n", program_path, prog_size_words);

    fprintf(trace_file, "--- INITIAL STATE ---\n");
    trace_regs_dump();
    printf("\n \n --- Initial Memory (0-64) ---\n \n");
    mem_dump(0, 64);
    fprintf(trace_file, "\n");
}

void trace_close(void) {
    if (trace_file && trace_file != stdout) fclose(trace_file);
}

void trace_regs_dump(void) {
    fprintf(trace_file, "--- Register File ---\n");
    char bin[33];
    for (int i = 0; i < NUM_REGS; i++) {
        int32_t v = regs_read(i);
        int_to_binary(v, bin);
        fprintf(trace_file, "R%d = %s (%d)\n", i, bin, v);
    }
    fprintf(trace_file, "\n");
}

// Contains the full print_detailed_cycle() from before – just renamed
void trace_cycle(uint32_t pc, uint32_t raw_instr, DecodedInstr ins,
                 int32_t rs1_val, int32_t rs2_val,
                 int32_t alu_result, int32_t mem_data, int32_t wb_data,
                 int cycle_num, bool mem_read, bool mem_write) {
    char bin[33];
    int_to_binary(raw_instr, bin);

    fprintf(trace_file, "=============================================================\n");
    fprintf(trace_file, "CYCLE %d    PC = 0x%04x\n", cycle_num, pc);
    fprintf(trace_file, "-------------------------------------------------------------\n");
    fprintf(trace_file, "FETCH:\n");
    fprintf(trace_file, "    Raw instruction bits = %s\n", bin);

    // Reconstruct human-readable assembly
    char rd_str[8] = "", rs1_str[8] = "", rs2_str[8] = "", imm_str[32] = "";
    if (ins.rd >= 0)  snprintf(rd_str,  sizeof(rd_str),  "%d", ins.rd);
    if (ins.rs1 >= 0) snprintf(rs1_str, sizeof(rs1_str), "%d", ins.rs1);
    if (ins.rs2 >= 0) snprintf(rs2_str, sizeof(rs2_str), "%d", ins.rs2);
    if (ins.type == I_TYPE || ins.type == S_TYPE) {
        snprintf(imm_str, sizeof(imm_str), "%d(R%d)", ins.imm, ins.rs1);
    }

    fprintf(trace_file, "    Disassembled        = ");
    switch (ins.op) {
        case OP_ADD:  fprintf(trace_file, "ADD R%s, R%s, R%s\n", rd_str, rs1_str, rs2_str); break;
        case OP_ADDI: fprintf(trace_file, "ADDI R%s, R%s, %d\n", rd_str, rs1_str, ins.imm); break;
        case OP_LW:   fprintf(trace_file, "LW R%s, %s\n", rd_str, imm_str); break;
        case OP_LB:   fprintf(trace_file, "LB R%s, %s\n", rd_str, imm_str); break;
        case OP_SW:   fprintf(trace_file, "SW R%s, %s\n", rs2_str, imm_str); break;
        case OP_SB:   fprintf(trace_file, "SB R%s, %s\n", rs2_str, imm_str); break;
        default:      fprintf(trace_file, "NOP\n"); break;
    }

    // DECODE details
    fprintf(trace_file, "\nDECODE:\n");
    uint32_t opcode = raw_instr & 0x7F;
    fprintf(trace_file, "    opcode = %02x → ", opcode);
    switch(opcode) {
        case 0x03: fprintf(trace_file, "LOAD\n"); break;
        case 0x13: fprintf(trace_file, "OP-IMM\n"); break;
        case 0x23: fprintf(trace_file, "STORE\n"); break;
        case 0x33: fprintf(trace_file, "OP (R-type)\n"); break;
        default:   fprintf(trace_file, "UNKNOWN\n"); break;
    }
    if (ins.rd  >= 0) fprintf(trace_file, "    rd  = R%-2d\n", ins.rd);
    if (ins.rs1 >= 0) fprintf(trace_file, "    rs1 = R%-2d\n", ins.rs1);
    if (ins.rs2 >= 0) fprintf(trace_file, "    rs2 = R%-2d\n", ins.rs2);
    if (ins.imm != 0 || ins.type == I_TYPE || ins.type == S_TYPE)
        fprintf(trace_file, "    imm = %d (sign-extended)\n", ins.imm);

    // Full bit field breakdown
    fprintf(trace_file, "    Full 32-bit layout:\n");
    fprintf(trace_file, "        31-20      19-15  14-12  11-7   6-0\n");
    fprintf(trace_file, "        imm        rs1    funct3 rd     opcode\n");
    fprintf(trace_file, "        ");
    for (int i = 31; i >= 0; i--) {
        fprintf(trace_file, "%c", (raw_instr & (1u << i)) ? '1' : '0');
        if (i == 20 || i == 15 || i == 12 || i == 7) fprintf(trace_file, "  ");
    }
    fprintf(trace_file, "\n\n");

    // Register Read
    fprintf(trace_file, "REGISTER READ:\n");
    if (ins.rs1 >= 0) {
        char valbin[33]; int_to_binary(rs1_val, valbin);
        fprintf(trace_file, "    R%d value → %s (%d)\n", ins.rs1, valbin, rs1_val);
    }
    if (ins.rs2 >= 0) {
        char valbin[33]; int_to_binary(rs2_val, valbin);
        fprintf(trace_file, "    R%d value → %s (%d)\n", ins.rs2, valbin, rs2_val);
    }

    // Execute
    fprintf(trace_file, "\nEXECUTE (ALU / Address Generation):\n");
    if (ins.type == R_TYPE)
        fprintf(trace_file, "    ALU: R%d + R%d = %d\n", ins.rs1, ins.rs2, alu_result);
    else
        fprintf(trace_file, "    Address = R%d + imm = %d + %d = 0x%04x (%d)\n",
                ins.rs1, rs1_val, ins.imm, alu_result, alu_result);

    // Memory
    if (mem_read || mem_write) {
        fprintf(trace_file, "\nMEMORY ACCESS:\n");
        if (mem_read) {
            char binmem[33]; int_to_binary(mem_data, binmem);
            fprintf(trace_file, "    Load from 0x%04x → %s (%d)%s\n",
                    alu_result, binmem, mem_data, ins.op == OP_LB ? " (byte, sign-extended)" : "");
        }
        if (mem_write) {
            char binmem[33]; int_to_binary(rs2_val, binmem);
            fprintf(trace_file, "    Store to 0x%04x ← %s (%d)%s\n",
                    alu_result, binmem, rs2_val, ins.op == OP_SB ? " (byte)" : "");
        }
    }

    // Writeback
    if (ins.rd > 0 && (ins.op == OP_ADD || ins.op == OP_ADDI || ins.op == OP_LW || ins.op == OP_LB)) {
        fprintf(trace_file, "\nWRITEBACK:\n");
        char binwb[33]; int_to_binary(wb_data, binwb);
        fprintf(trace_file, "    R%d ← %s (%d)\n", ins.rd, binwb, wb_data);
    }

    fprintf(trace_file, "\n");
    regs_dump_detailed(trace_file);
    fprintf(trace_file, "-------------------------------------------------------------\n\n");
}

void trace_final(int total_cycles) {
    fprintf(trace_file, "=== EXECUTION COMPLETE ===\n");
    fprintf(trace_file, "Total cycles: %d (CPI = 1.0)\n\n", total_cycles);
    fprintf(trace_file, "--- FINAL REGISTERS ---\n");
    trace_regs_dump();
    printf("\n \n --- FINAL MEMORY (0-64) ---\n \n ");
    mem_dump(0, 64);
}