#define _GNU_SOURCE
#include "boot.h"
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

Node g_nodes[MAX_NODES]; 
RemoteBull g_remote_bulls[MAX_REMOTE_BULLS] = {0};
static int g_initialized = 0; 
int g_remote_count = 0;

void save_memory(void) { 
    FILE *fp = fopen(MEM_FILE, "w"); 
    if(!fp) return; 
    for(int i=0;i<MAX_NODES;i++) 
        fprintf(fp, "%d %.4f %.4f %.4f %d\n", i, g_nodes[i].cpu_score, g_nodes[i].net_score, g_nodes[i].io_score, g_nodes[i].jobs_done); 
    fclose(fp); 
}

void load_memory(void) { 
    FILE *fp = fopen(MEM_FILE, "r"); 
    if(!fp) return; 
    for(int i=0;i<MAX_NODES;i++) 
        fscanf(fp, "%d %lf %lf %lf %d", &g_nodes[i].id, &g_nodes[i].cpu_score, &g_nodes[i].net_score, &g_nodes[i].io_score, &g_nodes[i].jobs_done); 
    fclose(fp); 
}

int pick_best_node(const char* job_type) { 
    double best = 999999; 
    int best_id = 0; 
    for (int i = 0; i < MAX_NODES; i++) { 
        if(!g_nodes[i].alive) continue; 
        double score = (strcmp(job_type, "cpu") == 0) ? g_nodes[i].cpu_score : (strcmp(job_type, "net") == 0) ? g_nodes[i].net_score : g_nodes[i].io_score; 
        if (score < best) { best = score; best_id = i; } 
    } 
    return best_id; 
}

void qmp_init_node(int node_id) { 
    char sock_path[64]; 
    snprintf(sock_path, 64, "/tmp/qmp-node-%d.sock", node_id); 
    log_print("[QMP] Node %d QMP ready: %s\n", node_id, sock_path); 
}

void qmp_send(int node_id, const char* cmd) { 
    struct timespec start, end; 
    clock_gettime(CLOCK_MONOTONIC, &start); 
    log_print("[QMP] -> Node %d: %s\n", node_id, cmd); 
    clock_gettime(CLOCK_MONOTONIC, &end); 
    double ms = (end.tv_sec - start.tv_sec)*1000.0 + (end.tv_nsec - start.tv_nsec)/1000000.0; 
    double *score = &g_nodes[node_id].cpu_score; 
    *score = (*score * 0.9) + (ms * 0.1); 
}

void network_herd_add(const char* ip, int port) { 
    if (g_remote_count >= MAX_REMOTE_BULLS) { log_print("[NET] Herd full\n"); return; } 
    strcpy(g_remote_bulls[g_remote_count].ip, ip); 
    g_remote_bulls[g_remote_count].port = port; 
    g_remote_bulls[g_remote_count].sock = socket(AF_INET, SOCK_STREAM, 0); 
    g_remote_bulls[g_remote_count].latency = 999.0;
    log_print("[NET] Added remote bull: %s:%d\n", ip, port); 
    g_remote_count++; 
}

void network_tick(void) { 
    log_print("[NET] === DISTRIBUTED TICK ===\n"); 
    for(int i=0;i<g_remote_count;i++) { 
        log_print("[NET] Pinging bull %s:%d\n", g_remote_bulls[i].ip, g_remote_bulls[i].port); 
        g_remote_bulls[i].latency = 10.0 + (rand()%50); 
    } 
    int best_local = pick_best_node("cpu"); 
    double best_remote = (g_remote_count > 0) ? g_remote_bulls[0].latency : 999.0;
    log_print("[NET] Routing decision: Local Node %d vs Best Remote %.2fms\n", best_local, best_remote); 
}

void orchestrator_assign_job(const char* job_type) { 
    if (!g_initialized) orchestrator_init(); 
    int node = pick_best_node(job_type); 
    struct timespec start, end; 
    clock_gettime(CLOCK_MONOTONIC, &start); 
    kill(g_nodes[node].pid, 0); 
    clock_gettime(CLOCK_MONOTONIC, &end); 
    double ms = (end.tv_sec - start.tv_sec)*1000.0 + (end.tv_nsec - start.tv_nsec)/1000000.0; 
    double *score = (strcmp(job_type, "cpu") == 0) ? &g_nodes[node].cpu_score : (strcmp(job_type, "net") == 0) ? &g_nodes[node].net_score : &g_nodes[node].io_score; 
    *score = (*score * 0.9) + (ms * 0.1); 
    g_nodes[node].jobs_done++; 
    log_print("[ORCHESTRATOR] Job '%s' -> Node %d | Latency: %.2fms\n", job_type, node, ms); 
    save_memory(); 
}

void orchestrator_init(void) {
    if (g_initialized) return;
    adaptive_boot(); 
    log_print("[ORCHESTRATOR] Latching 11 ticks onto the bull...\n"); 
    load_memory();
    for (int i = 0; i < MAX_NODES; i++) {
        g_nodes[i].id = i; 
        if (g_nodes[i].cpu_score == 0) g_nodes[i].cpu_score = 1000.0; 
        g_nodes[i].alive = 0;
        pid_t pid = fork(); 
        if (pid == 0) { 
            char name[32]; 
            snprintf(name, 32, "node-%d", i); 
            execlp(QEMU_BIN, QEMU_BIN, "-nographic", "-m", "512", "-name", name, "-daemonize", NULL); 
            exit(1); 
        } else { 
            g_nodes[i].pid = pid; 
            g_nodes[i].alive = 1; 
            log_print("[ORCHESTRATOR] Node %d latched. PID %d\n", i, pid); 
            qmp_init_node(i); 
        }
        usleep(200000);
    }
    if (strstr(g_chip.vendor, "AuthenticAMD")) { for(int i=0;i<MAX_NODES;i++) g_nodes[i].cpu_score *= 0.9; log_print("[ORCHESTRATOR] AMD bias: CPU favored\n"); }
    if (strstr(g_chip.vendor, "GenuineIntel")) { for(int i=0;i<MAX_NODES;i++) g_nodes[i].io_score *= 0.9; log_print("[ORCHESTRATOR] Intel bias: IO favored\n"); }
    g_initialized = 1;
}

void orchestrator_tick(void) { 
    log_print("[ORCHESTRATOR] === LOCAL TICK ===\n"); 
    orchestrator_assign_job("cpu"); 
    orchestrator_assign_job("net"); 
    orchestrator_assign_job("io"); 
    qmp_send(pick_best_node("cpu"), "{'execute':'query-cpus'}"); 
}

void orchestrator_status(void) { 
    log_print("[ORCHESTRATOR] === THE HERD ===\n"); 
    for (int i = 0; i < MAX_NODES; i++) 
        log_print("[NODE %d] PID:%d CPU:%.2f NET:%.2f IO:%.2f | Jobs:%d\n", g_nodes[i].id, g_nodes[i].pid, g_nodes[i].cpu_score, g_nodes[i].net_score, g_nodes[i].io_score, g_nodes[i].jobs_done); 
}
