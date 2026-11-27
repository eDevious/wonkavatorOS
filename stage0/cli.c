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
    log_print("[LEVIATHAN] Type 'help' for all v0.1-v0.8 commands\n\n");

    while (1) {
        printf("[LEVIATHAN] > ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) break;
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "help") == 0) {
            log_print("v0.1-v0.8 Commands:\n");
            log_print("  scan      - Quick scan: CPU/MEM/NET\n");
            log_print("  manifest  - Full scan + save snapshot\n");
            log_print("  probe     - Deep system probe /proc /sys\n");
            log_print("  parasite  - Start process resource monitor\n");
            log_print("  tracer    - Run 1 trace cycle\n");
            log_print("  tick      - Orchestrator tick. Assign jobs to 11 nodes\n");
            log_print("  herd      - Show all 11 node bull status\n");
            log_print("  info      - System summary\n");
            log_print("  hosts     - List saved manifests\n");
            log_print("  load      - Load a manifest: load manifest_xxx.txt\n");
            log_print("  diff      - Compare to last snapshot\n");
            log_print("  export    - Export to CSV\n");
            log_print("  watch     - Auto-scan every 5s\n");
            log_print("  stop      - Stop watch/parasite\n");
            log_print("  exit      - Quit\n");
        } 
        else if (strcmp(line, "scan") == 0) { quick_scan(); }
        else if (strcmp(line, "manifest") == 0) { full_manifest(); }
        else if (strcmp(line, "probe") == 0) { probe_system(); }
        else if (strcmp(line, "parasite") == 0) { 
            if (!g_parasite) { g_parasite = 1; pthread_create(&parasite_tid, NULL, parasite_thread, NULL); log_print("[PARASITE] Started.\n"); }
        }
        else if (strcmp(line, "tracer") == 0) { tracer_log("manual_trace", 0.0); }
        else if (strcmp(line, "tick") == 0) { orchestrator_tick(); }
        else if (strcmp(line, "herd") == 0) { orchestrator_status(); }
        else if (strcmp(line, "info") == 0) { print_host_info(); print_cpu_info(); print_mem_info(); }
        else if (strcmp(line, "hosts") == 0) { list_snapshots(); }
        else if (strncmp(line, "load ", 5) == 0) { load_snapshot(line + 5); }
        else if (strcmp(line, "diff") == 0) { diff_snapshot(); }
        else if (strcmp(line, "export") == 0) { export_manifest(); }
        else if (strcmp(line, "watch") == 0) {
            if (!g_watching) { g_watching = 1; pthread_create(&watch_tid, NULL, watch_thread, NULL); log_print("[WATCH] Started.\n"); }
        }
        else if (strcmp(line, "stop") == 0) {
            if (g_watching) { g_watching = 0; pthread_join(watch_tid, NULL); log_print("[WATCH] Stopped\n"); }
            if (g_parasite) { g_parasite = 0; pthread_join(parasite_tid, NULL); log_print("[PARASITE] Stopped\n"); }
        }
        else if (strcmp(line, "exit") == 0) {
            if (g_watching) { g_watching = 0; pthread_join(watch_tid, NULL); }
            if (g_parasite) { g_parasite = 0; pthread_join(parasite_tid, NULL); }
            break;
        } 
        else if (strlen(line) > 0) { log_print("Unknown: %s\n", line); }
    }
}

void* watch_thread(void* arg) { while (g_watching) { log_print("\n[WATCH] === AUTO SCAN ===\n"); quick_scan(); sleep(5); } return NULL; }
void* parasite_thread(void* arg) { while (g_parasite) { parasite_monitor(); sleep(2); } return NULL; }
