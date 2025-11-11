#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

void int_to_binary(uint32_t value, char *binary_str, int bits);

uint32_t binary_to_int(const char *binary_str);

void print_bin_label(const char *label, const char *binary_str);

#endif