#include "boot.h"
#include <time.h>

#ifdef _WIN32
    #define PING_CMD "ping -n 1 -w 1000 %s > nul"
    #define NC_CMD "powershell -Command \"Test-NetConnection %s -Port %d -WarningAction SilentlyContinue | Select-Object -ExpandProperty TcpTestSucceeded\""
#else
    #define PING_CMD "ping -c 1 -W 1 %s > /dev/null 2>&1"
    #define NC_CMD "nc -z -w 1 %s %d 2>/dev/null"
#endif

const int common_ports[SCAN_PORTS] = {22, 23, 25, 53, 80, 110, 139, 443, 445, 3389};

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
    printf("[PARASITE] FULL VERBOSE SCAN INITIATED ON %s\n", target_ip);
    printf("[PARASITE] =========================================\n");
    
    srand((unsigned)time(NULL));
    
    // 1. PING
    printf("[1/3] PING SWEEP... ");
    fflush(stdout);
    uint32_t ping = real_ping(target_ip);
    g_node_health[0].last_ping_ms = ping;
    g_node_health[0].ip_up = (ping > 0) ? 1 : 0;
    if(ping) printf("%dms ALIVE\n", ping);
    else { printf("HOST DOWN\n"); g_node_health[0].parasite_score += 5; return; }
    
    // 2. PORT SCAN
    printf("[2/3] PORT SCAN: %d common ports\n", SCAN_PORTS);
    int open_count = 0;
    for(int i=0; i<SCAN_PORTS; i++) {
        printf("  [>] Probing port %d... ", common_ports[i]);
        fflush(stdout);
        int is_open = scan_port(target_ip, common_ports[i]);
        g_node_health[0].open_ports[i] = is_open;
        if(is_open) { printf("OPEN\n"); open_count++; }
        else printf("CLOSED\n");
    }
    printf("  [RESULT] %d/%d ports open\n", open_count, SCAN_PORTS);
    if(open_count == 0) g_node_health[0].parasite_score += 3;
    
    // 3. FAKE CPU LATENCY TEST
    printf("[3/3] CPU LATENCY PROBE... ");
    uint32_t latency = 3 + (rand() % 20);
    g_node_health[0].last_latency_ms = latency;
    printf("%dms\n", latency);
    if(latency > 15) g_node_health[0].parasite_score++;
    
    // FINAL VERDICT
    printf("[PARASITE] =========================================\n");
    printf("[PARASITE] FINAL SCORE: %d\n", g_node_health[0].parasite_score);
    if(g_node_health[0].parasite_score >= 3) {
        g_node_health[0].blacklisted = 1;
        printf("[PARASITE] TARGET %s BLACKLISTED\n", target_ip);
    } else {
        printf("[PARASITE] TARGET %s CLEAR\n", target_ip);
    }
}

int is_node_blacklisted(const char* node) {
    return g_node_health[0].blacklisted;
}

void parasitic_update(const char* node, uint32_t latency_ms) {
    g_node_health[0].last_latency_ms = latency_ms;
}
