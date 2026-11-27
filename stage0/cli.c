#include "boot.h"
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdio.h>

int g_watching = 0; 
int g_parasite = 0;
int g_scanning = 0; // NEW: kill switch for scanport
char g_scan_target[32] = {0}; // NEW: target or "ALL"
pthread_t watch_tid; 
pthread_t parasite_tid;
pthread_t scan_tid; // NEW

#define SCAN_TIMEOUT_MS 200

void grab_banner(int sock, char* buf, size_t len) {
    fd_set fds; struct timeval tv;
    FD_ZERO(&fds); FD_SET(sock, &fds); 
    tv.tv_sec = 1; tv.tv_usec = 0;
    if (select(sock+1, &fds, NULL, NULL, &tv) > 0) {
        recv(sock, buf, len-1, MSG_DONTWAIT);
    }
    buf[len-1] = 0;
}

void* portscan_thread(void* arg) { // NEW: threaded scanner
    char* ip = (char*)arg;
    
    if (strcmp(ip, "ALL") == 0) { // CARPET BOMB
        log_print("[PORTSCAN] === CARPET BOMB MODE: Scanning all live hosts ===\n");
        
        FILE* fp = popen("ip -o -f inet addr show | awk '{print $4}' | head -1", "r");
        char subnet[32] = "192.168.1";
        if (fp) { 
            char myip[32]; 
            fgets(myip, 32, fp); 
            pclose(fp); 
            char* dot = strrchr(myip, '.'); 
            if (dot) *dot = 0; 
            strncpy(subnet, myip, 31); 
            subnet[31] = 0;
        }
        
        for (int i = 1; i < 255 && g_scanning; i++) { // CHECK FLAG EACH HOST
            char target[32];
            snprintf(target, sizeof(target), "%s.%d", subnet, i);
            
            char cmd[128];
            snprintf(cmd, sizeof(cmd), "ping -c 1 -W 1 %s > /dev/null 2>&1", target);
            if (system(cmd) == 0) {
                log_print("\n[PORTSCAN] >>> TARGET ACQUIRED: %s <<<\n", target);
                print_open_ports(target);
            }
        }
        log_print("[PORTSCAN] === CARPET BOMB COMPLETE ===\n");
    } else {
        print_open_ports(ip); // SINGLE TARGET
    }
    
    g_scanning = 0;
    return NULL;
}

void print_open_ports(const char* ip) {
    log_print("[PORTSCAN] === VERBOSE SCAN %s 1-65535 ===\n", ip);
    int open_count = 0;
    
    for (int port = 1; port <= 65535 && g_scanning; port++) { // CHECK FLAG EACH PORT
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) continue;
        
        int flags = fcntl(sock, F_GETFL, 0);
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);
        
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, ip, &addr.sin_addr);
        
        connect(sock, (struct sockaddr*)&addr, sizeof(addr));
        
        fd_set wfds; struct timeval tv;
        FD_ZERO(&wfds); FD_SET(sock, &wfds);
        tv.tv_sec = 0; tv.tv_usec = SCAN_TIMEOUT_MS * 1000;
        
        if (select(sock+1, NULL, &wfds, NULL, &tv) > 0) {
            int err = 0; socklen_t len = sizeof(err);
            getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &len);
            if (err == 0) {
                open_count++;
                char banner[256] = {0};
                grab_banner(sock, banner, sizeof(banner));
                
                const char* service = "unknown";
                if (port == 22) service = "ssh";
                else if (port == 80) service = "http";
                else if (port == 443) service = "https";
                else if (port == 445) service = "smb";
                else if (port == 3389) service = "rdp";
                else if (port == 5900) service = "vnc";
                else if (port == 6379) service = "redis";
                else if (port == 5432) service = "postgres";
                
                log_print("[PORT] %5d/tcp OPEN  %-10s | Banner: %.40s\n", port, service, banner);
            }
        }
        close(sock);
    }
    if (g_scanning) log_print("[PORTSCAN] === END %s: %d ports open ===\n", ip, open_count);
    else log_print("[PORTSCAN] === ABORTED %s ===\n", ip);
}

void cli_loop(void) {
    char line[256];
    log_print("[WONKAVATOR] Running initial manifest...\n");
    full_manifest();
    log_print("[LEVIATHAN] Type 'help' for all v0.1-v0.9.3 commands\n\n");

    while (1) {
        printf("[LEVIATHAN] > "); fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) break;
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "help") == 0) {
            log_print("v0.1-v0.9.3 Commands:\n");
            log_print("  scan/manifest/info/hosts/load/diff/export - Core scans\n");
            log_print("  probe - Deep /proc /sys probe\n");
            log_print("  parasite/stop - Process monitor on/off\n");
            log_print("  watch/stop - Auto scan on/off\n");
            log_print("  tracer - Function timing\n");
            log_print("  chip - CPU/GPU/NIC detection\n");
            log_print("  boot - Adaptive boot loader\n");
            log_print("  tick - Local orchestrator tick via QMP\n");
            log_print("  herd - Show 11 node status\n");
            log_print("  addbull IP PORT - Add remote bull\n");
            log_print("  ntick - Distributed tick across network\n");
            log_print("  qmp - Send QMP to fastest node\n");
            log_print("  scanip - Adaptive IP discovery\n");
            log_print("  scanport [IP] - Port scan 1-65535. No IP = scan all hosts\n");
            log_print("  stop - Stop watch/parasite/scanport\n");
            log_print("  exit - Shutdown\n");
        } 
        else if (strcmp(line, "scan") == 0) { quick_scan(); }
        else if (strcmp(line, "manifest") == 0) { full_manifest(); }
        else if (strcmp(line, "probe") == 0) { probe_system(); }
        else if (strcmp(line, "parasite") == 0) { 
            if (!g_parasite) { 
                g_parasite = 1; 
                pthread_create(&parasite_tid, NULL, parasite_thread, NULL); 
                log_print("[PARASITE] Started.\n"); 
            } 
        }
        else if (strcmp(line, "tracer") == 0) { tracer_log("manual_trace", 0.0); }
        else if (strcmp(line, "chip") == 0) { chip_detect(); }
        else if (strcmp(line, "boot") == 0) { adaptive_boot(); }
        else if (strcmp(line, "tick") == 0) { orchestrator_tick(); }
        else if (strcmp(line, "herd") == 0) { orchestrator_status(); }
        else if (strncmp(line, "addbull ", 8) == 0) { 
            char ip[64]; int port; 
            sscanf(line+8, "%s %d", ip, &port); 
            network_herd_add(ip, port); 
        }
        else if (strcmp(line, "ntick") == 0) { network_tick(); }
        else if (strcmp(line, "qmp") == 0) { qmp_send(pick_best_node("cpu"), "{'execute':'query-status'}"); }
        else if (strcmp(line, "scanip") == 0) { adaptive_net_discovery(); }
        else if (strncmp(line, "scanport", 8) == 0) { 
            if (g_scanning) {
                log_print("[PORTSCAN] Already running. Use 'stop' to abort.\n");
            } else {
                g_scanning = 1;
                char* ip = line + 9;
                if (strlen(ip) == 0) strncpy(g_scan_target, "ALL", 31);
                else strncpy(g_scan_target, ip, 31);
                g_scan_target[31] = 0;
                pthread_create(&scan_tid, NULL, portscan_thread, g_scan_target);
                log_print("[PORTSCAN] Started. Type 'stop' to abort.\n");
            }
        }
        else if (strcmp(line, "info") == 0) { print_host_info(); print_cpu_info(); print_mem_info(); }
        else if (strcmp(line, "hosts") == 0) { list_snapshots(); }
        else if (strncmp(line, "load ", 5) == 0) { load_snapshot(line + 5); }
        else if (strcmp(line, "diff") == 0) { diff_snapshot(); }
        else if (strcmp(line, "export") == 0) { export_manifest(); }
        else if (strcmp(line, "watch") == 0) { 
            if (!g_watching) { 
                g_watching = 1; 
                pthread_create(&watch_tid, NULL, watch_thread, NULL); 
                log_print("[WATCH] Started.\n"); 
            } 
        }
        else if (strcmp(line, "stop") == 0) { 
            if (g_scanning) {
                g_scanning = 0;
                pthread_join(scan_tid, NULL);
                log_print("[PORTSCAN] Stopped\n");
            }
            if (g_watching) { 
                g_watching = 0; 
                pthread_join(watch_tid, NULL); 
                log_print("[WATCH] Stopped\n"); 
            } 
            if (g_parasite) { 
                g_parasite = 0; 
                pthread_join(parasite_tid, NULL); 
                log_print("[PARASITE] Stopped\n"); 
            } 
        }
        else if (strcmp(line, "exit") == 0) { 
            g_scanning = 0; if (g_scanning) pthread_join(scan_tid, NULL);
            if (g_watching) { 
                g_watching = 0; 
                pthread_join(watch_tid, NULL); 
            } 
            if (g_parasite) { 
                g_parasite = 0; 
                pthread_join(parasite_tid, NULL); 
            } 
            break; 
        } 
        else if (strlen(line) > 0) { 
            log_print("Unknown: %s\n", line); 
        }
    }
}

void* watch_thread(void* arg) { 
    while (g_watching) { 
        log_print("\n[WATCH] === AUTO SCAN ===\n"); 
        quick_scan(); 
        sleep(5); 
    } 
    return NULL; 
}

void* parasite_thread(void* arg) { 
    while (g_parasite) { 
        parasite_monitor(); 
        sleep(2); 
    } 
    return NULL; 
}
