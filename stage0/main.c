#include "boot.h"

int main(int argc, char *argv[]) {
    printf("[WONKAVATOR] Forging WonkavatorOS v0.7 LEVIATHAN...\n");
    printf("[WONKAVATOR] All systems consolidated. Booting CLI.\n\n");
    cli_loop();
    printf("[WONKAVATOR] Shutdown complete.\n");
    return 0;
}
3. stage0/cli.c
c
#include "boot.h"
#include <string.h>
#include <pthread.h>
#include <unistd.h>

int g_watching = 0;
pthread_t watch_tid;

void cli_loop(void) {
    char line[256];
    log_print("[WONKAVATOR] Running initial manifest...\n");
    full_manifest();
    log_print("[LEVIATHAN] Type 'help' for all commands\n\n");

    while (1) {
        printf("[LEVIATHAN] > ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) break;
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "help") == 0) {
            log_print("Commands:\n");
            log_print("  manifest  - Run full hardware scan + snapshot\n");
            log_print("  scan      - Alias for manifest\n");
            log_print("  info      - Quick system summary\n");
            log_print("  diff      - Compare to last snapshot\n");
            log_print("  export    - Export manifest to CSV\n");
            log_print("  watch     - Start auto-scan every 5s\n");
            log_print("  stop      - Stop watch\n");
            log_print("  exit      - Quit\n");
        } 
        else if (strcmp(line, "manifest") == 0 || strcmp(line, "scan") == 0) {
            full_manifest();
        }
        else if (strcmp(line, "info") == 0) {
            print_host_info();
            print_cpu_info();
            print_mem_info();
        }
        else if (strcmp(line, "diff") == 0) {
            diff_snapshot();
        }
        else if (strcmp(line, "export") == 0) {
            export_manifest();
        }
        else if (strcmp(line, "watch") == 0) {
            if (!g_watching) {
                g_watching = 1;
                pthread_create(&watch_tid, NULL, watch_thread, NULL);
                log_print("[WATCH] Started. Scanning every 5s. Type 'stop' to end.\n");
            } else {
                log_print("[WATCH] Already running\n");
            }
        }
        else if (strcmp(line, "stop") == 0) {
            if (g_watching) {
                g_watching = 0;
                pthread_join(watch_tid, NULL);
                log_print("[WATCH] Stopped\n");
            }
        }
        else if (strcmp(line, "exit") == 0) {
            if (g_watching) { g_watching = 0; pthread_join(watch_tid, NULL); }
            log_print("[LEVIATHAN] Shutting down.\n");
            break;
        } 
        else if (strlen(line) > 0) {
            log_print("Unknown command: %s\n", line);
        }
    }
}

void* watch_thread(void* arg) {
    while (g_watching) {
        log_print("\n[WATCH] === AUTO SCAN ===\n");
        create_snapshot();
        sleep(5);
    }
    return NULL;
}
