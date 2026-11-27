#define _GNU_SOURCE
#include "boot.h"
#include <sys/socket.h> #include <netinet/in.h> #include <arpa/inet.h>
#include <netdb.h> #include <fcntl.h> #include <errno.h>

void print_open_ports(const char* ip) {
    log_print("[SCAN] Probing %s for open ports...\n", ip);
    int common_ports[] = {22, 80, 443, 9000, 9001, 3389, 5900, 8080, 0};
    for(int i=0; common_ports[i] != 0; i++) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in addr; addr.sin_family = AF_INET; addr.sin_port = htons(common_ports[i]);
        inet_pton(AF_INET, ip, &addr.sin_addr);
        fcntl(sock, F_SETFL, O_NONBLOCK);
        connect(sock, (struct sockaddr*)&addr, sizeof(addr));
        usleep(100000);
        int err = 0; socklen_t len = sizeof(err); getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &len);
        if (err == 0) log_print("[SCAN]   [OPEN]  %s:%d\n", ip, common_ports[i]);
        close(sock);
    }
}

void adaptive_net_discovery(void) {
    log_print("[ADMIRALTY] === ADAPTIVE IP DISCOVERY ===\n");
    struct ifaddrs *ifaddr, *ifa; 
    if (getifaddrs(&ifaddr) == -1) return;
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            char ip[INET_ADDRSTRLEN]; inet_ntop(AF_INET, &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr, ip, INET_ADDRSTRLEN);
            log_print("[ADMIRALTY] Interface %s | IP: %s\n", ifa->ifa_name, ip);
            char subnet[32]; strcpy(subnet, ip); char *last_dot = strrchr(subnet, '.'); if(last_dot) *last_dot = 0;
            log_print("[ADMIRALTY] Scanning subnet: %s.1-254\n", subnet);
            for(int i=1; i<255; i++) {
                char target[32]; snprintf(target, 32, "%s.%d", subnet, i);
                int sock = socket(AF_INET, SOCK_STREAM, 0);
                struct sockaddr_in addr; addr.sin_family = AF_INET; addr.sin_port = htons(22);
                inet_pton(AF_INET, target, &addr.sin_addr);
                fcntl(sock, F_SETFL, O_NONBLOCK);
                if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
                    log_print("[ADMIRALTY] [HOST UP] %s\n", target);
                    print_open_ports(target);
                }
                close(sock);
            }
        }
    }
    freeifaddrs(ifaddr);
    log_print("[ADMIRALTY] === DISCOVERY COMPLETE ===\n\n");
}

void ip_port_scan(void) {
    char line[128]; log_print("Enter target IP or 'all' for subnet: ");
    fgets(line, sizeof(line), stdin); line[strcspn(line, "\n")] = 0;
    if(strcmp(line, "all") == 0) adaptive_net_discovery();
    else print_open_ports(line);
}
