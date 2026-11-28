#ifndef BOOT_H
#define BOOT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define LOG_FILE "wonkavator.log"

static inline void log_print(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    
    FILE* f = fopen(LOG_FILE, "a");
    if (f) {
        va_start(args, fmt);
        vfprintf(f, fmt, args);
        va_end(args);
        fclose(f);
    }
}

// hardware.c
void quick_scan(void);
void full_manifest(void);
void probe_system(void);
void print_host_info(void);
void print_cpu_info(void);
void print_mem_info(void);
void list_snapshots(void);
void load_snapshot(const char* name);
void diff_snapshot(void);
void export_manifest(void);
void parasite_monitor(void);

// orchestrator.c
void orchestrator_tick(void);
void orchestrator_status(void);
void network_herd_add(const char* ip, int port);
void network_tick(void);
const char* pick_best_node(const char* metric);
void qmp_send(const char* node, const char* cmd);

// net_scan.c
void adaptive_net_discovery(void);
int is_host_up(const char* ip);

// cli.c
void cli_loop(void);
void* watch_thread(void* arg);
void* parasite_thread(void* arg);

// hardware_fingerprint.c - NEW v0.9.2
void fingerprint_hardware(void);
void fingerprint_microcontrollers(void);
void fingerprint_firmware_remote(const char* ip);

// tracer.c - stub
static inline void tracer_log(const char* name, double ms) { (void)name; (void)ms; }

// chip.c - stub
static inline void chip_detect(void) { log_print("[CHIP] Detection stub\n"); }

// boot.c - stub  
static inline void adaptive_boot(void) { log_print("[BOOT] Adaptive boot stub\n"); }

#endif
