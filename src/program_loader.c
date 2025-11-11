#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "isa.h"
#include "utils.h"

// Convert opcode string to respective defined opcode enum 
static inline Opcode opcode_from_str(const char *s) {
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
int parse_reg(const char *tok) {
    if (!tok || tok[0] != 'R') return -1; 
    int r = tok[1] - '0';
    return (r >= 0 && r < NUM_REGS) ? r : -1;
}

// Breaking one line of instruction into instruction format
int parse_line(const char *line, Instr *ins) {
    char buf[INST_SIZE];
    strcpy(buf, line);
    char *tok = strtok(buf, " \t\n,");  // split on space/comma
    if (!tok) return -1;

    *ins = (Instr){ OP_UNKNOWN, 0, 0, 0, 0 };
    ins->op = opcode_from_str(tok);

    char *tokens[4];
    int i = 0;
    while ((tok = strtok(NULL, " \t\n,")) && i < 4)
        tokens[i++] = tok;

    // Parse based on instruction type
    if (ins->op == OP_LW || ins->op == OP_LB) {
        ins->rd = parse_reg(tokens[0]);
        ins->imm = atoi(tokens[1]);
    }
    else if (ins->op == OP_SW || ins->op == OP_SB) {
        ins->rs1 = parse_reg(tokens[0]);
        ins->imm = atoi(tokens[1]);
    }
    else if (ins->op == OP_ADD) {
        ins->rd = parse_reg(tokens[0]);
        ins->rs1 = parse_reg(tokens[1]);
        ins->rs2 = parse_reg(tokens[2]);
    }
    else if (ins->op == OP_ADDI) {
        ins->rd = parse_reg(tokens[0]);
        ins->rs1 = parse_reg(tokens[1]);
        ins->imm = atoi(tokens[2]);
    }
    else if (ins->op == OP_NOP) {
        return 0;
    } else return -1;
    
    return 0;
}

// Load program from file 
int load_program(const char *path, Instr prog[], int *count) {
    if (!path || !prog || !count) return -1;

    FILE *f = fopen(path, "r");
    if (!f) return -1;

    char line[INST_SIZE];
    int i = 0;

    while (fgets(line, sizeof(line), f) && i < NUM_INST) {
        if (line[0] == '#' || line[0] == '\n') continue;
        Instr ins;
        if (parse_line(line, &ins) == 0)
            prog[i++] = ins;
    }

    fclose(f);
    *count = i;
    return 0;
}
