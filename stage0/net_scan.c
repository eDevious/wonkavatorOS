#include "boot.h"

/* NETWORK SCANNING v0.9.1 - Win32 sockets + Linux ping */
void adaptive_net_discovery(void) {
    log_print("[NET] Adaptive discovery starting on 192.168.1.0/24\n");
    for (int i = 1; i < 5; i++) {
        char ip[32];
        snprintf(ip, sizeof(ip), "192.168.1.%d", i);
        if (is_host_up(ip)) {
            log_print("[NET] Host UP: %s\n", ip);
            print_open_ports(ip);
        }
    }
}

/* Cross-platform host check */
int is_host_up(const char* ip) {
    #ifdef _WIN32
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "ping -n 1 -w 500 %s", ip);
    #else
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "ping -c 1 -W 1 %s", ip);
    #endif
    
    FILE* p = popen(cmd, "r");
    if (!p) return 0;
    int ret = (pclose(p) == 0);
    return ret;
}

void print_open_ports(const char* ip) {
    log_print("[NET] Port scan stub for %s. Future: socket connect scan\n", ip);
}
