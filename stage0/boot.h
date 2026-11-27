#ifndef BOOT_H
#define BOOT_H
#include <stdio.h> #include <stdint.h> #include <arpa/inet.h> #include <pthread.h> #include <time.h>

void log_print(const char* fmt, ...);

#define MAX_NODES 11
#define MAX_REMOTE_BULLS 16
#define QEMU_BIN "qemu-system-x86_64"
#define MEM_FILE ".orch_memory"

typedef struct {
    char vendor[64]; char model[128]; int cores; int threads; double ghz;
    char gpu_vendor[64]; char gpu_model[128];
} ChipInfo;

typedef struct {
    int id; pid_t pid; double cpu_score; double net_score; double io_score;
    int alive; int jobs_done;
} Node;

typedef struct {
    char ip[64]; int port; int sock; double latency; int active;
} RemoteBull;

extern ChipInfo g_chip;
extern Node g_nodes[MAX_NODES];
extern RemoteBull g_remote_bulls[MAX_REMOTE_BULLS];
extern int g_remote_count;

// Core v0.1-v0.6
void print_host_info(void); void print_cpu_info(void); void print_mem_info(void);
void print_local_hardware_verbose(void); void scan_pci_devices(void); void scan_usb_devices(void);
void print_dmi_info(void); void create_snapshot(void); void diff_snapshot(void);
void list_snapshots(void); void load_snapshot(const char* fname); void export_manifest(void);
void quick_scan(void); void full_manifest(void);

// v0.7
void probe_system(void); void parasite_monitor(void); void tracer_log(const char* func, double ms);

// v0.8
void chip_detect(void); void adaptive_boot(void);

// v0.9 Core
void orchestrator_init(void); void orchestrator_tick(void); void orchestrator_status(void);
void qmp_init_node(int node_id); void qmp_send(int node_id, const char* cmd);
void network_herd_add(const char* ip, int port); void network_tick(void);
int pick_best_node(const char* job_type); void orchestrator_assign_job(const char* job_type);
void save_memory(void); void load_memory(void);

// v0.9.2 Admiralty
void ip_port_scan(void); void adaptive_net_discovery(void); void print_open_ports(const char* ip);

// CLI
extern int g_watching; extern int g_parasite; extern pthread_t watch_tid; extern pthread_t parasite_tid;
void* watch_thread(void* arg); void* parasite_thread(void* arg); void cli_loop(void);
#endif
