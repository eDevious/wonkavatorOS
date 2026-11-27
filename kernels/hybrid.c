#kernels/hybrid.c
c
#include <stdint.h>
volatile uint32_t* uart = (uint32_t*)0x40000000;
void _start() { *uart = 0x48; while(1); }