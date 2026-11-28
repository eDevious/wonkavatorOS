#include "boot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_host_up(const char* ip) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "ping -c 1 -W 1 %s > /dev/null 2>&1", ip);
    return system(cmd) == 0;
}

void adaptive_net_discovery(void) {
    log_print("[NETSCAN] === ADAPTIVE NETWORK DISCOVERY ===\n");
    
    FILE* fp = popen("ip -o -f inet addr show | awk '{print $4}' | head -1", "r");
    char subnet[32] = "192.168.1";
    if (fp) { 
        char myip[32]; 
        if (fgets(myip, 32, fp)) {
            myip[strcspn(myip, "/")] = 0;
            char* dot = strrchr(myip, '.'); 
            if (dot) *dot = 0; 
            strncpy(subnet, myip, 31); 
            subnet[31] = 0;
        }
        pclose(fp); 
    }
    
    log_print("[NETSCAN] Probing subnet: %s.0/24\n", subnet);
    int hosts_found = 0;
    for (int i = 1; i < 255; i++) {
        char target[32]; 
        snprintf(target, sizeof(target), "%s.%d", subnet, i);
        if (is_host_up(target)) {
            hosts_found++;
            log_print("[NETSCAN] HOST UP: %s\n", target);
        }
    }
    log_print("[NETSCAN] === DISCOVERY COMPLETE: %d hosts ===\n", hosts_found);
}
