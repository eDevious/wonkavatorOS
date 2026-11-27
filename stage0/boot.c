#include "boot.h"
#include <string.h>

worker_t g_workers[MAX_WORKERS];
placement_entry_t g_placement_table[PLACEMENT_TABLE_SIZE];
int g_placement_count = 0;
node_health_t g_node_health[MAX_WORKERS];

int main(void) {
    printf("Wonkavator OS Stage0 v0.6\n");
    
    memset(g_workers, 0, sizeof(g_workers));
    memset(g_placement_table, 0, sizeof(g_placement_table));
    memset(g_node_health, 0, sizeof(g_node_health));
    
    ai_placer_load();
    dma_init();
    
    // EDIT THESE TO YOUR REAL LAN IPs
    const char* ip_list[11] = {
        "192.168.170.183",
    };

    for(int i=0; i<11; i++) { 
        snprintf(g_workers[i].id, 8, "N-%02d", i); 
        g_workers[i].online = 1; 
        g_workers[i].cpu_mhz = 2400;
        
        snprintf(g_node_health[i].node_id, 8, "N-%02d", i);
        strncpy(g_node_health[i].ip, ip_list[i], 15);
        g_node_health[i].ip[15] = '\0';
        g_node_health[i].parasite_score = 0;
        g_node_health[i].last_latency_ms = 0;
        g_node_health[i].last_ping_ms = 0;
        g_node_health[i].blacklisted = 0;
        g_node_health[i].ip_up = 1;
    }
    
    printf("11 nodes ready. Type 'help' for commands.\n\n");
    cli_loop();
    
    ai_placer_save();
    return 0;
}
