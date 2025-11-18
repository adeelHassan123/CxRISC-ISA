#ifndef TRACE_H
#define TRACE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "isa.h"

static FILE *trace_file = NULL;

void trace_init(const char *program_path, int prog_size_words);
void trace_close(void);
void trace_regs_dump(void);
void trace_cycle(uint32_t pc, uint32_t raw_instr, DecodedInstr ins,
                 int32_t rs1_val, int32_t rs2_val,
                 int32_t alu_result, int32_t mem_data, int32_t wb_data,
                 int cycle_num, bool mem_read, bool mem_write);
void trace_final(int total_cycles);

#endif