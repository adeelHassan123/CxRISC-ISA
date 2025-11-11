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
    if (!line || !ins) return -1;

    char op[16], a[16], b[16], c[16];
    int n = sscanf(line, "%15s %15[^,], %15[^,], %15s", op, a, b, c);
    if (n <= 0) return -1;

    *ins = (Instr){ OP_UNKNOWN, 0, 0, 0, 0 };
    ins->op = opcode_from_str(op);

    // simple decoding based on opcode
    if (ins->op == OP_LB || ins->op == OP_LW) {
        ins->rd  = parse_reg(a);
        ins->imm = atoi(b);
    } else if (ins->op == OP_SB || ins->op == OP_SW) {
        ins->rs1 = parse_reg(a);
        ins->imm = atoi(b);
    } else if (ins->op == OP_ADD || ins->op == OP_ADDI) {
        ins->rd  = parse_reg(a);
        ins->rs1 = parse_reg(b);
        ins->rs2 = parse_reg(c);
    }

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
