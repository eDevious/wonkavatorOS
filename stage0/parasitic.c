#include "boot.h"
#include <time.h>

void parasitic_scan(void) {
    printf("[PARASITE] Scanning 11 nodes for latency anomalies...\n");
    srand((unsigned)time(NULL));
    for(int i=0; i<11; i++) {
        uint32_t latency = 3 + (rand() % 20); // simulate ping
        g_node_health[i].last_latency_ms = latency;
        
        if(latency > 15) { 
            g_node_health[i].parasite_score++; 
            printf("[PARASITE] %s SLOW: %dms - score:%d\n", g_workers[i].id, latency, g_node_health[i].parasite_score);
        }
        if(g_node_health[i].parasite_score >= 3) {
            g_node_health[i].blacklisted = 1;
            printf("[PARASITE] %s BLACKLISTED - Too parasitic\n", g_workers[i].id);
        }
    }
}

int is_node_blacklisted(const char* node) {
    for(int i=0; i<11; i++) if(strcmp(g_node_health[i].node_id, node) == 0) return g_node_health[i].blacklisted;
    return 0;
}

void parasitic_update(const char* node, uint32_t latency_ms) {
    for(int i=0; i<11; i++) if(strcmp(g_node_health[i].node_id, node) == 0) {
        g_node_health[i].last_latency_ms = latency_ms;
        if(latency_ms < 10 && g_node_health[i].parasite_score > 0) g_node_health[i].parasite_score--; // forgiveness
    }
}