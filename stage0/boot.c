#include "boot.h"

worker_t g_workers[MAX_WORKERS];
placement_entry_t g_placement_table[PLACEMENT_TABLE_SIZE];
int g_placement_count = 0;

int main(void) {
    printf("Wonkavator OS Stage0 v0.4-ai-placer\n");
    ai_placer_load();
    dma_init();
    for(int i=0; i<11; i++) { sprintf(g_workers[i].id, "N-%02d", i); g_workers[i].online = 1; }
    printf("11 nodes ready. Type 'help' for commands.\n\n");
    cli_loop();
    return 0;
}