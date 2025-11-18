#include "decode.h"
#include "isa.h"
#include "utils.h"

DecodedInstr decode(uint32_t raw_instr) {
    DecodedInstr ins = {OP_UNKNOWN, R_TYPE, -1, -1, -1, 0};
    uint32_t opcode = raw_instr & 0x7F;
    uint32_t funct3 = (raw_instr >> 12) & 0x7;

    switch (opcode) {
        case OPC_OP:        // 0x33 - ADD only for now
            ins.type = R_TYPE;
            ins.op = OP_ADD;
            ins.rd  = (raw_instr >> 7)  & 0x1F;
            ins.rs1 = (raw_instr >> 15) & 0x1F;
            ins.rs2 = (raw_instr >> 20) & 0x1F;
            break;
        case OPC_OPIMM:     // 0x13 - ADDI
            ins.type = I_TYPE;
            ins.op = OP_ADDI;
            ins.rd  = (raw_instr >> 7)  & 0x1F;
            ins.rs1 = (raw_instr >> 15) & 0x1F;
            ins.imm = sign_extend(raw_instr >> 20, 12);
            break;
        case OPC_LOAD:      // 0x03 - LW/LB
            ins.type = I_TYPE;
            ins.rd  = (raw_instr >> 7)  & 0x1F;
            ins.rs1 = (raw_instr >> 15) & 0x1F;
            ins.imm = sign_extend(raw_instr >> 20, 12);
            ins.op = (funct3 == 0x0) ? OP_LB : OP_LW;
            break;
        case OPC_STORE:     // 0x23 - SW/SB
            ins.type = S_TYPE;
            ins.rs1 = (raw_instr >> 15) & 0x1F;
            ins.rs2 = (raw_instr >> 20) & 0x1F;
            ins.imm = sign_extend(((raw_instr >> 25) << 5) | ((raw_instr >> 7) & 0x1F), 12);
            ins.op = (funct3 == 0x0) ? OP_SB : OP_SW;
            break;
        case 0x00:
            ins.op = OP_NOP;
            break;
    }
    return ins;
}