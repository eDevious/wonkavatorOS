#include "boot.h"

/* Called every 5s by watch_thread */
void orchestrator_tick(void) {
    static int tick = 0;
    log_print("[ORCH] Tick %d\n", tick++);
    network_tick();
}

void orchestrator_status(void) {
    log_print("[ORCH] Status: IDLE | Nodes in herd: 0\n");
}

/* Add a remote node to the herd for distributed tasks */
void network_herd_add(const char* ip, int port) {
    log_print("[ORCH] Adding node %s:%d to herd\n", ip, port);
}

/* Future: ping all nodes in herd */
void network_tick(void) {
    // TODO: iterate herd and check health
}

/* Scheduler: pick node with most free RAM/CPU */
const char* pick_best_node(const char* metric) {
    (void)metric;
    return "localhost"; // v0.9.1 only has localhost
}

/* Send QMP command to remote QEMU node */
void qmp_send(const char* node, const char* cmd) {
    log_print("[QMP] Sending '%s' to %s\n", cmd, node);
}
