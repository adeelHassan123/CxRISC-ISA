#ifndef REGISTERS_H
#define REGISTERS_H

void regs_init();                   
int regs_read(int index);            
void regs_write(int index, int val); 
void regs_dump();      

#endif