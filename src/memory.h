#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stdbool.h>

void mem_init();                     
int32_t dmem_read(uint32_t addr, bool is_byte);
void dmem_write(uint32_t addr, int32_t value, bool is_byte);
void mem_dump(int start_byte, int end_byte);

#endif