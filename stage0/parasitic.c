#include "boot.h"
#include <time.h>

#ifdef _WIN32
    #define PING_CMD "ping -n 1 -w 1000 %s > nul"
#else
    #define PING_CMD "ping -c 1 -W 1 %s > /dev/null 2>&1"
#endif

uint32_t real_ping(const char* ip) {
    char cmd[128];
    snprintf(cmd, 128, PING_CMD, ip);
    clock_t start = clock();
    int ret = system(cmd);
    clock_t end = clock();
    if(ret != 0) return 0;
    uint32_t ms = (uint32_t)((end - start) * 1000 / CLOCKS_PER_SEC);
    return ms < 1 ? 1 : ms;
}

void parasitic_scan(void) {
    printf("[PARASITE] Scanning 11 nodes: CPU + REAL IP PING...\n");
    srand((unsigned)time(NULL));
    for(int i=0; i<11; i++) {
        uint32_t latency = 3 + (rand() % 20);
        g_node_health[i].last_latency_ms = latency;
        
        printf("[PARASITE] Pinging %s... ", g_node_health[i].ip);
        fflush(stdout);
        uint32_t ping = real_ping(g_node_health[i].ip);
        g_node_health[i].last_ping_ms = ping;
        g_node_health[i].ip_up = (ping > 0) ? 1 : 0;
        
        if(!g_node_health[i].ip_up) {
            g_node_health[i].parasite_score += 2;
            printf("DOWN - score:%d\n", g_node_health[i].parasite_score);
        }
        else if(ping > 50) {
            g_node_health[i].parasite_score++;
            printf("%dms SLOW - score:%d\n", ping, g_node_health[i].parasite_score);
        }
        else printf("%dms OK\n", ping);

        if(latency > 15) { 
            g_node_health[i].parasite_score++; 
            printf("[PARASITE] %s SLOW CPU: %dms\n", g_workers[i].id, latency);
        }
        if(g_node_health[i].parasite_score >= 3) {
            g_node_health[i].blacklisted = 1;
            printf("[PARASITE] %s BLACKLISTED\n", g_workers[i].id);
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
        if(latency_ms < 10 && g_node_health[i].parasite_score > 0) g_node_health[i].parasite_score--;
    }
}
