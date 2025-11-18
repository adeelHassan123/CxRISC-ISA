#include "execute.h"
#include "registers.h"
#include "memory.h"

ExecuteResult execute_instruction(DecodedInstr ins, int32_t rs1_val, int32_t rs2_val) {
    ExecuteResult res = {0};

    // EXECUTE STAGE (ALU + Address Generation)
    int32_t operand2 = (ins.type == I_TYPE || ins.type == S_TYPE) ? ins.imm : rs2_val;
    res.alu_result = rs1_val + operand2;  

    // MEMORY STAGE
    res.mem_read  = (ins.op == OP_LW || ins.op == OP_LB);
    res.mem_write = (ins.op == OP_SW || ins.op == OP_SB);

    if (res.mem_read) {
        res.mem_data = dmem_read(res.alu_result, ins.op == OP_LB);
    }
    if (res.mem_write) {
        dmem_write(res.alu_result, rs2_val, ins.op == OP_SB);
    }

    // Prepare data for writeback
    res.wb_data = res.mem_read ? res.mem_data : res.alu_result;

    return res;
}

void execute_writeback(DecodedInstr ins, int32_t wb_data) {
    if (ins.rd > 0 && 
        (ins.op == OP_ADD || ins.op == OP_ADDI || 
         ins.op == OP_LW  || ins.op == OP_LB)) {
        regs_write(ins.rd, wb_data);
    }
}