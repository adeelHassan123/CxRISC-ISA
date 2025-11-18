#ifndef EXECUTE_H
#define EXECUTE_H

#include <stdint.h>
#include <stdbool.h>
#include "isa.h"

typedef struct {
    int32_t alu_result;
    int32_t mem_data;
    int32_t wb_data;
    bool    mem_read;
    bool    mem_write;
} ExecuteResult;

ExecuteResult execute_instruction(DecodedInstr ins, int32_t rs1_val, int32_t rs2_val);
void execute_writeback(DecodedInstr ins, int32_t wb_data);

#endif