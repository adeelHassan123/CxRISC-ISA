#ifndef ISA_H
#define ISA_H

#include <stdint.h>
#include <stdbool.h>

#define NUM_REGS 8
#define IMEM_SIZE 256    // Instructions (32-bit words) 256 × 4 bytes = 1024 bytes
#define DMEM_SIZE 1024   // Data memory in bytes
#define INST_SIZE 128    //Length of each instrucion line in asm code
#define WORD_BITS 32     // 32-bit architecture

typedef enum { //enum for unique numeric ID
    OP_NOP = 0,
    OP_LB,
    OP_LW,
    OP_SB,
    OP_SW,
    OP_ADD,
    OP_ADDI,
    OP_UNKNOWN
} Opcode;

typedef enum {
    R_TYPE,
    I_TYPE,
    S_TYPE,
    B_TYPE,
    U_TYPE,
    J_TYPE
} InstrType;

typedef struct {
    Opcode op; 
    InstrType  type; 
    int rd;     
    int rs1;    
    int rs2;    
    int32_t    imm;  
} DecodedInstr;

// RISC-V Base Opcode Map (TEMPORARY - Needs complete Lookup Match Table)
#define OPC_LOAD    0x03
#define OPC_OPIMM   0x13
#define OPC_STORE   0x23
#define OPC_OP      0x33

#endif