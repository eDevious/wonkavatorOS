#include "boot.h"
#include <stdio.h>
#include <string.h>

#define MAX_NODES 16
typedef struct {
    char ip[64];
    int port;
    int active;
} Node;

static Node g_herd[MAX_NODES];
static int g_node_count = 0;

void orchestrator_tick(void) {
    log_print("[ORCH] === ORCHESTRATOR TICK ===\n");
    log_print("[ORCH] Managing %d nodes\n", g_node_count);
}

void orchestrator_status(void) {
    log_print("[ORCH] === HERD STATUS ===\n");
    if (g_node_count == 0) {
        log_print("[ORCH] No nodes in herd\n");
        return;
    }
    for (int i = 0; i < g_node_count; i++) {
        log_print("[ORCH] Node %d: %s:%d %s\n", 
            i, g_herd[i].ip, g_herd[i].port, g_herd[i].active ? "ACTIVE" : "DOWN");
    }
}

void network_herd_add(const char* ip, int port) {
    if (g_node_count >= MAX_NODES) {
        log_print("[ORCH] Herd full\n");
        return;
    }
    strncpy(g_herd[g_node_count].ip, ip, 63);
    g_herd[g_node_count].ip[63] = 0;
    g_herd[g_node_count].port = port;
    g_herd[g_node_count].active = 1;
    g_node_count++;
    log_print("[ORCH] Added node: %s:%d\n", ip, port);
}

void network_tick(void) {
    log_print("[ORCH] === NETWORK TICK ===\n");
    for (int i = 0; i < g_node_count; i++) {
        g_herd[i].active = is_host_up(g_herd[i].ip);
    }
}

const char* pick_best_node(const char* metric) {
    (void)metric;
    if (g_node_count == 0) return "localhost";
    for (int i = 0; i < g_node_count; i++) {
        if (g_herd[i].active) return g_herd[i].ip;
    }
    return "localhost";
}

void qmp_send(const char* node, const char* cmd) {
    log_print("[QMP] Sending to %s: %s\n", node, cmd);
    // Stub: would open socket to QEMU QMP port
}
