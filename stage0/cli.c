#include "boot.h"
#include <ctype.h>

/* LEVIATHAN COMMAND LOOP v0.9.1.1 
 * All features now have a command and appear in help
 */
void cli_loop(void) {
    char cmd[256];
    log_print("wonkavator v0.9.1.1 READY. Type 'help'\n");
    log_print("wonkavator> ");
    
    while (fgets(cmd, sizeof(cmd), stdin)) {
        cmd[strcspn(cmd, "\n")] = 0; // Strip newline
        char* arg = strchr(cmd, ' '); // Split command and args
        if(arg){ *arg = 0; arg++; }
        
        if (strcmp(cmd, "help") == 0) {
            log_print("=== WONKAVATOR v0.9.1.1 COMMANDS ===\n");
            log_print(" hw      : scan       - Quick CPU/RAM scan\n");
            log_print(" hw      : manifest   - Full hardware manifest\n");
            log_print(" hw      : probe      - Probe sensors\n");
            log_print(" hw      : export     - Export manifest to json\n");
            log_print(" hw      : fp         - Fingerprint hardware\n");
            log_print(" hw      : fp-mcu     - Scan for ESP32/STM32/Pico\n");
            log_print(" snap    : snap-list  - List snapshots\n");
            log_print(" snap    : snap-load <name> - Load snapshot\n");
            log_print(" snap    : snap-diff  - Diff current vs last\n");
            log_print(" net     : net        - Adaptive network discovery\n");
            log_print(" orch    : orch-status- Show orchestrator status\n");
            log_print(" orch    : herd-add <ip> <port> - Add node to herd\n");
            log_print(" orch    : qmp <node> <cmd> - Send QMP command\n");
            log_print(" sys     : stop       - Shutdown\n");
        } 
        else if (strcmp(cmd, "scan") == 0) { quick_scan(); }
        else if (strcmp(cmd, "manifest") == 0) { full_manifest(); }
        else if (strcmp(cmd, "probe") == 0) { probe_system(); }
        else if (strcmp(cmd, "export") == 0) { export_manifest(); }
        else if (strcmp(cmd, "fp") == 0) { fingerprint_hardware(); }
        else if (strcmp(cmd, "fp-mcu") == 0) { fingerprint_microcontrollers(); }
        else if (strcmp(cmd, "snap-list") == 0) { list_snapshots(); }
        else if (strcmp(cmd, "snap-load") == 0) { load_snapshot(arg ? arg : "default"); }
        else if (strcmp(cmd, "snap-diff") == 0) { diff_snapshot(); }
        else if (strcmp(cmd, "net") == 0) { adaptive_net_discovery(); }
        else if (strcmp(cmd, "orch-status") == 0) { orchestrator_status(); }
        else if (strcmp(cmd, "herd-add") == 0) { 
            if(arg){ char ip[64]; int port; if(sscanf(arg, "%s %d", ip, &port)==2) network_herd_add(ip, port); else log_print("Usage: herd-add <ip> <port>\n");}
            else log_print("Usage: herd-add <ip> <port>\n");
        }
        else if (strcmp(cmd, "qmp") == 0) {
            if(arg){ char node[64], qcmd[128]; if(sscanf(arg, "%s %[^\n]", node, qcmd)==2) qmp_send(node, qcmd); else log_print("Usage: qmp <node> <cmd>\n");}
            else log_print("Usage: qmp <node> <cmd>\n");
        }
        else if (strcmp(cmd, "stop") == 0) { log_print("Stopping...\n"); break; }
        else { log_print("Unknown command: %s. Type 'help'\n", cmd); }
        log_print("wonkavator> ");
    }
}

/* Background thread: runs orchestrator every 5 seconds */
THREAD_RETURN watch_thread(void* arg) {
    (void)arg;
    while(1) {
        orchestrator_tick();
        sleep(5);
    }
    return 0;
}

/* Background thread: resource parasite monitor */
THREAD_RETURN parasite_thread(void* arg) {
    (void)arg;
    parasite_monitor();
    return 0;
}
