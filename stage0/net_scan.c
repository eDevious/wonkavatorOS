#include "boot.h"

/* Scan local /24 for live hosts */
void adaptive_net_discovery(void) {
    log_print("[NET] Adaptive discovery starting on 192.168.1.0/24\n");
    for (int i = 1; i < 10; i++) { // Scan first 10 for speed
        char ip[32];
        snprintf(ip, sizeof(ip), "192.168.1.%d", i);
        if (is_host_up(ip)) {
            log_print("[NET] Host UP: %s\n", ip);
            print_open_ports(ip);
        }
    }
}

/* Cross-platform ping using popen */
int is_host_up(const char* ip) {
    #ifdef _WIN32
    char cmd[128]; snprintf(cmd, sizeof(cmd), "ping -n 1 -w 500 %s", ip);
    #else
    char cmd[128]; snprintf(cmd, sizeof(cmd), "ping -c 1 -W 1 %s", ip);
    #endif
    FILE* p = popen(cmd, "r");
    if (!p) return 0;
    int ret = (pclose(p) == 0); // 0 = success
    return ret;
}

/* Basic TCP connect scan for common ports */
void print_open_ports(const char* ip) {
    log_print("[NET] Scanning %s for 80,443,22,3389...\n", ip);
    int ports[] = {80, 443, 22, 3389};
    
    #ifdef _WIN32
    WSADATA wsa; WSAStartup(MAKEWORD(2,2), &wsa);
    #endif
    
    for(int i=0; i<4; i++){
        socket_t s = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in addr; 
        addr.sin_family = AF_INET; 
        addr.sin_port = htons(ports[i]);
        inet_pton(AF_INET, ip, &addr.sin_addr);
        
        // Non-blocking connect with 500ms timeout would be better
        if(connect(s, (struct sockaddr*)&addr, sizeof(addr)) == 0){
            log_print("[NET]   PORT %d: OPEN\n", ports[i]);
        }
        #ifdef _WIN32
        closesocket(s);
        #else
        close(s);
        #endif
    }
    
    #ifdef _WIN32
    WSACleanup();
    #endif
}
