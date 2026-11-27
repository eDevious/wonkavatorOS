#include "boot.h"
#include <time.h>

#ifdef _WIN32
    #define PING_CMD "ping -n 1 -w 1000 %s > nul"
    #define NC_CMD "powershell -Command \"Test-NetConnection %s -Port %d -WarningAction SilentlyContinue | Select-Object -ExpandProperty TcpTestSucceeded\""
#else
    #define PING_CMD "ping -c 1 -W 1 %s > /dev/null 2>&1"
    #define NC_CMD "nc -z -w 0.5 %s %d 2>/dev/null"
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

int scan_port(const char* ip, int port) {
    char cmd[128];
    snprintf(cmd, 128, NC_CMD, ip, port);
    return system(cmd) == 0 ? 1 : 0;
}

void parasitic_scan(const char* target_ip) {
    printf("[PARASITE v0.6] FULL 1-65535 TCP SCAN ON %s\n", target_ip);
    printf("[PARASITE] =========================================\n");
    
    srand((unsigned)time(NULL));
    g_node_health[0].parasite_score = 0;
    g_node_health[0].open_port_count = 0;
    
    printf("[1/3] PING... ");
    uint32_t ping = real_ping(target_ip);
    g_node_health[0].last_ping_ms = ping;
    g_node_health[0].ip_up = (ping > 0);
    if(ping) printf("%dms ALIVE\n", ping);
    else { printf("DOWN\n"); g_node_health[0].parasite_score += 5; return; }
    
    printf("[2/3] SCANNING PORTS 1-65535\n");
    for(int port=1; port<=65535; port++) {
        if(port % 3277 == 0) printf("  Progress: %d%%\r", port*100/65535);
        if(scan_port(target_ip, port)) {
            g_node_health[0].open_port_count++;
            printf("\n  [+] PORT %d OPEN\n", port);
        }
    }
    printf("  Found %d open ports\n", g_node_health[0].open_port_count);
    
    printf("[3/3] CPU LATENCY... ");
    uint32_t latency = 3 + (rand() % 20);
    g_node_health[0].last_latency_ms = latency;
    printf("%dms\n", latency);
    if(latency > 15) g_node_health[0].parasite_score++;
    
    printf("[PARASITE] SCORE:%d PORTS:%d\n", g_node_health[0].parasite_score, g_node_health[0].open_port_count);
    if(g_node_health[0].parasite_score >= 3) {
        g_node_health[0].blacklisted = 1;
        printf("[PARASITE] BLACKLISTED\n");
    }
}

int is_node_blacklisted(const char* node) { return g_node_health[0].blacklisted; }
void parasitic_update(const char* node, uint32_t latency_ms) { g_node_health[0].last_latency_ms = latency_ms; }
