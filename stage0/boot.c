#include "boot.h"

worker_t g_workers[MAX_WORKERS];
placement_entry_t g_placement_table[PLACEMENT_TABLE_SIZE];
int g_placement_count = 0;
node_health_t g_node_health[MAX_WORKERS]; // NEW v0.5

int main(void) {
    printf("Wonkavator OS Stage0 v0.5-parasite\n"); // CHANGED v0.5
    ai_placer_load();
    dma_init();
    for(int i=0; i<11; i++) { 
        sprintf(g_workers[i].id, "N-%02d", i); 
        g_workers[i].online = 1; 
        strcpy(g_node_health[i].node_id, g_workers[i].id); // NEW v0.5
        g_node_health[i].parasite_score = 0;
        g_node_health[i].blacklisted = 0;
    }
    printf("11 nodes ready. Type 'help' for commands.\n\n");
    cli_loop();
    return 0;
}