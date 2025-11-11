#ifndef MEMORY_H
#define MEMORY_H

void mem_init();                     
int mem_read(int addr);            
void mem_write(int addr, int val);   
void mem_dump(int start, int end);

#endif