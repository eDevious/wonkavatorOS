#include "boot.h"

/* HARDWARE SCANNING v0.9.1 - Cross Platform */
void quick_scan(void) {
    log_print("[HW] Quick scan starting...\n");
    print_host_info();
    print_cpu_info();
    print_mem_info();
}

void full_manifest(void) {
    log_print("[HW] Full hardware manifest\n");
    quick_scan();
    // Future: enumerate USB, PCI, etc
}

void probe_system(void) {
    log_print("[HW] Probing system sensors...\n");
    #ifdef _WIN32
    // TODO: WMI calls for CPU temp, etc
    log_print("[HW] Win32 WMI probe not yet implemented\n");
    #else
    // TODO: /proc and /sys parsing
    log_print("[HW] Linux /proc probe not yet implemented\n");
    #endif
}

void print_host_info(void) {
    log_print("[HOST] Hostname: ");
    #ifdef _WIN32
    char name[256]; DWORD len = 256; GetComputerNameA(name, &len); log_print("%s\n", name);
    #else
    char name[256]; gethostname(name, 256); log_print("%s\n", name);
    #endif
}

void print_cpu_info(void) { log_print("[CPU] CPU Info stub\n"); }
void print_mem_info(void) { log_print("[MEM] Memory Info stub\n"); }
void list_snapshots(void) { log_print("[SNAP] No snapshots yet\n"); }
void load_snapshot(const char* name) { (void)name; log_print("[SNAP] Load stub\n"); }
void diff_snapshot(void) { log_print("[SNAP] Diff stub\n"); }
void export_manifest(void) { log_print("[HW] Export to manifest.json stub\n"); }
void parasite_monitor(void) { log_print("[PARASITE] Monitor running\n"); sleep(10); }
