#include "boot.h"
#include <string.h>

worker_t g_workers[MAX_WORKERS];
placement_entry_t g_placement_table[PLACEMENT_TABLE_SIZE];
int g_placement_count = 0;
node_health_t g_node_health[MAX_WORKERS];

int main(void) {
    printf("Wonkavator OS Stage0 v0.6 [FULLSCAN]\n");
    
    memset(g_workers, 0, sizeof(g_workers));
    memset(g_placement_table, 0, sizeof(g_placement_table));
    memset(g_node_health, 0, sizeof(g_node_health));
    
    ai_placer_load();
    dma_init();
    
    snprintf(g_workers[0].id, 8, "N-00"); 
    g_workers[0].online = 1; 
    g_workers[0].cpu_mhz = 2400;
    snprintf(g_node_health[0].node_id, 8, "N-00");
    strcpy(g_node_health[0].ip, "192.168.170.183"); // CHANGE DEFAULT TARGET HERE
    
    printf("Target: %s. Usage: parasite 192.168.1.50\n\n", g_node_health[0].ip);
    cli_loop();
    
    ai_placer_save();
    return 0;
}
