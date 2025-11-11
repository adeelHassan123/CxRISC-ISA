#include <stdio.h>
#include "utils.h"

void int_to_binary(uint32_t value, char *binary_str, int bits) {
    for (int i = bits - 1; i >= 0; --i) {
        binary_str[bits - 1 - i] = ((value >> i) & 1) ? '1' : '0';
    }
    binary_str[bits] = '\0';
}

uint32_t binary_to_int(const char *binary_str) {
    uint32_t out = 0;
    for (const char *p = binary_str; *p; ++p) {
        out = (out << 1) + (uint32_t)(*p - '0');
    }
    return out;
}

void print_bin_label(const char *label, const char *binary_str) {
    printf("%s %s\n", label, binary_str);
}