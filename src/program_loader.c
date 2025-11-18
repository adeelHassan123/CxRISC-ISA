#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "isa.h"
#include "utils.h"

/*
Assembly Program (.asm text)
     ↓
parse_line()      // Convert each line into DecodedInstr
     ↓
encode_*_type()   // Convert DecodedInstr - 32-bit machine code
     ↓
load_program()    // Place machine code into instruction memory (imem)
*/

// Convert opcode string to respective defined opcode enum 
static Opcode opcode_from_str(const char *s) {
    if (!s) return OP_UNKNOWN;

    switch (s[0]) {
        case 'A':
            if (strcmp(s, "ADD") == 0) return OP_ADD;
            if (strcmp(s, "ADDI") == 0) return OP_ADDI;
            break;
        case 'S':
            if (strcmp(s, "SB") == 0) return OP_SB;
            if (strcmp(s, "SW") == 0) return OP_SW;
            break;
        case 'L':
            if (strcmp(s, "LB") == 0) return OP_LB;
            if (strcmp(s, "LW") == 0) return OP_LW;
            break;
        case 'N': if (strcmp(s, "NOP") == 0) return OP_NOP; break;
    }
    return OP_UNKNOWN;
}

// Convert register string to register's index value
static int parse_reg(const char *tok) {
    if (!tok || tok[0] != 'R') return -1; 
    int r = atoi(tok + 1);
    return (r >= 0 && r < NUM_REGS) ? r : -1;
}

static int32_t parse_imm_with_base(const char *tok, int *base_reg) {
    char buf[64];
    strncpy(buf, tok, sizeof(buf) - 1);
    buf[sizeof(buf)-1] = '\0';

    char *offset_str = buf;
    char *reg_str = strstr(buf, "(");
    if (reg_str) {
        *reg_str = '\0';
        reg_str++;
        char *close = strchr(reg_str, ')');
        if (close) *close = '\0';
        *base_reg = parse_reg(reg_str);
    } else {
        *base_reg = 0;
    }
    return atoi(offset_str);
}

static uint32_t encode_r_type(int rd, int rs1, int rs2) {
    return OPC_OP | (rd << 7) | (rs1 << 15) | (rs2 << 20);
}

static uint32_t encode_i_type(Opcode op, int rd, int rs1, int32_t imm) {
    uint32_t code = (op == OP_ADDI) ? OPC_OPIMM : OPC_LOAD;
    int funct3 = (op == OP_LB) ? 0 : 2;
    return code | (rd << 7) | (rs1 << 15) | (funct3 << 12) | ((imm & 0xFFF) << 20);
}

static uint32_t encode_s_type(Opcode op, int rs1, int rs2, int32_t imm) {
    int funct3 = (op == OP_SB) ? 0 : 2;
    uint32_t imm_low  = imm & 0x1F;
    uint32_t imm_high = (imm >> 5) & 0x7F;
    return OPC_STORE | (rs1 << 15) | (rs2 << 20) | (funct3 << 12) |
           (imm_low << 7) | (imm_high << 25);
}

// Breaking one line of instruction into instruction format
int parse_line(const char *line, DecodedInstr *ins) {
    char buf[INST_SIZE];
    strcpy(buf, line);

    char *tok = strtok(buf, " \t\n,");
    if (!tok) return -1;

    ins->op  = opcode_from_str(tok);
    ins->rd  = ins->rs1 = ins->rs2 = ins->imm = 0;

    char *tokens[4];
    int n = 0;

    while ((tok = strtok(NULL, " \t\n,")) && n < 4)
        tokens[n++] = tok;

    switch (ins->op) {

        case OP_ADD:
            if (n < 3) return -1;
            ins->rd  = parse_reg(tokens[0]);
            ins->rs1 = parse_reg(tokens[1]);
            ins->rs2 = parse_reg(tokens[2]);
            break;

        case OP_ADDI:
            if (n < 3) return -1;
            ins->rd  = parse_reg(tokens[0]);
            ins->rs1 = parse_reg(tokens[1]);
            ins->imm = atoi(tokens[2]);
            break;

        case OP_LW:
        case OP_LB:
            if (n < 2) return -1;
            ins->rd = parse_reg(tokens[0]);
            ins->imm = parse_imm_with_base(tokens[1], &ins->rs1);
            break;

        case OP_SW:
        case OP_SB:
            if (n < 2) return -1;
            ins->rs2 = parse_reg(tokens[0]);
            ins->imm = parse_imm_with_base(tokens[1], &ins->rs1);
            break;

        case OP_NOP:
            return 0;

        default:
            return -1;
    }

    return 0;
}

// Load program from file 
int load_program(const char *path, uint32_t imem[], int *count) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;

    char line[INST_SIZE];
    int i = 0;

    while (fgets(line, sizeof(line), f) && i < IMEM_SIZE) {
        // Skip empty or comment-only lines
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;

        if (*p == '#' || *p == '\n' || *p == '\0')
            continue;

        DecodedInstr din;
        if (parse_line(line, &din) != 0)
            continue;

        uint32_t encoded = 0;

        switch (din.op) {
            case OP_ADD:
                encoded = encode_r_type(din.rd, din.rs1, din.rs2);
                break;

            case OP_ADDI:
            case OP_LW:
            case OP_LB:
                encoded = encode_i_type(din.op, din.rd, din.rs1, din.imm);
                break;

            case OP_SW:
            case OP_SB:
                encoded = encode_s_type(din.op, din.rs1, din.rs2, din.imm);
                break;

            case OP_NOP:
                encoded = 0;
                break;

            default:
                continue;
        }

        imem[i++] = encoded;
    }

    fclose(f);
    *count = i;
    return 0;
}