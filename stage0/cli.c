#include "boot.h"
#include <ctype.h>

/* LEVIATHAN COMMAND LOOP v0.9.1 */
void cli_loop(void) {
    char cmd[256];
    log_print("wonkavator> ");
    while (fgets(cmd, sizeof(cmd), stdin)) {
        cmd[strcspn(cmd, "\n")] = 0; // strip newline
        
        if (strcmp(cmd, "help") == 0) {
            log_print("Commands: scan, manifest, probe, net, orch, snap, export, stop\n");
        } else if (strcmp(cmd, "scan") == 0) {
            quick_scan();
        } else if (strcmp(cmd, "manifest") == 0) {
            full_manifest();
        } else if (strncmp(cmd, "probe", 5) == 0) {
            probe_system();
        } else if (strncmp(cmd, "net", 3) == 0) {
            adaptive_net_discovery();
        } else if (strcmp(cmd, "stop") == 0) {
            log_print("Stopping...\n");
            break;
        } else {
            log_print("Unknown command: %s\n", cmd);
        }
        log_print("wonkavator> ");
    }
}

/* Background watcher thread */
THREAD_RETURN watch_thread(void* arg) {
    (void)arg;
    while(1) {
        orchestrator_tick();
        sleep(5);
    }
    return 0;
}

/* Background parasite monitor */
THREAD_RETURN parasite_thread(void* arg) {
    (void)arg;
    parasite_monitor();
    return 0;
}
