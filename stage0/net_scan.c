#include "boot.h"
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

// Helper: ping a host with 1 packet
int is_host_up(const char* ip) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "ping -c 1 -W 1 %s > /dev/null 2>&1", ip);
    return system(cmd) == 0;
}

void adaptive_net_discovery(void) {
    log_print("[SCANIP] === ADAPTIVE IP DISCOVERY ===\n");
    FILE* fp;
    
    fp = popen("ip -o -f inet addr show | awk '{print $4}' | head -1", "r");
    char subnet[32] = "192.168.1";
    if (fp) { 
        char ip[32]; 
        fgets(ip, 32, fp); 
        pclose(fp); 
        char* dot = strrchr(ip, '.'); 
        if (dot) *dot = 0; 
        strncpy(subnet, ip, 31); 
        subnet[31] = 0;
    }
    
    log_print("[SCANIP] Scanning subnet: %s.1-254\n", subnet);
    
    for (int i = 1; i < 255; i++) {
        char target[32];
        snprintf(target, sizeof(target), "%s.%d", subnet, i);
        if (is_host_up(target)) {
            log_print("[SCANIP] HOST UP: %s\n", target);
        }
    }
    log_print("[SCANIP] === END ===\n");
}
