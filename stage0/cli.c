#include "boot.h"
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>

int g_watching = 0; 
int g_parasite = 0;
pthread_t watch_tid; 
pthread_t parasite_tid;

void cli_loop(void) {
    char line[256];
    log_print("[WONKAVATOR] Running initial manifest...\n");
    full_manifest();
    log_print("[LEVIATHAN] Type 'help' for all v0.1-v0.9 commands\n\n");

    while (1) {
        printf("[LEVIATHAN] > "); fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) break;
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "help") == 0) {
            log_print("v0.1-v0.9 Commands:\n");
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
            log_print("  scanport IP - Verbose port scan\n");
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
        else if (strncmp(line, "scanport ", 9) == 0) { print_open_ports(line + 9); }
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
