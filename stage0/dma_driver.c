#include "boot.h"

void dma_init(void) { printf("[DMA] 3 channels initialized at 0x50000000\n"); }
void dma_memcpy(int ch, void* dst, void* src, size_t len) { memcpy(dst, src, len); printf("[DMA] Copied %d bytes\n", (int)len); }