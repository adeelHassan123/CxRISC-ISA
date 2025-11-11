#ifndef ISA_H
#define ISA_H

#define NUM_REGS 8
#define MEM_SIZE 512
#define NUM_INST 256 //Number of instructions in one program
#define INST_SIZE 128 //Length of each instrucion line in asm code
#define WORD_BITS    32         // 32-bit architecture

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

typedef struct {
    Opcode op;  
    int rd;     
    int rs1;    
    int rs2;    
    int imm;   
} Instr;

#endif 