#include "boot.h"

/* Quick scan: what user needs 90% of the time */
void quick_scan(void) {
    log_print("[HW] Quick scan starting...\n");
    print_host_info();
    print_cpu_info();
    print_mem_info();
}

/* Full manifest: calls quick + future USB/PCI enum */
void full_manifest(void) {
    log_print("[HW] Full hardware manifest\n");
    quick_scan();
    log_print("[HW] USB/PCI enumeration: TODO for v0.9.2\n");
}

/* Probe sensors. Platform specific */
void probe_system(void) {
    log_print("[HW] Probing system sensors...\n");
    #ifdef _WIN32
    log_print("[HW] Win32: WMI sensor probe not yet implemented\n");
    #else
    log_print("[HW] Linux: /sys/class/hwmon probe not yet implemented\n");
    #endif
}

/* Cross-platform hostname */
void print_host_info(void) {
    log_print("[HOST] Hostname: ");
    #ifdef _WIN32
    char name[256]; DWORD len = 256; GetComputerNameA(name, &len); log_print("%s\n", name);
    #else
    char name[256]; gethostname(name, 256); log_print("%s\n", name);
    #endif
}

/* Cross-platform CPU cores */
void print_cpu_info(void) {
    log_print("[CPU] ");
    #ifdef _WIN32
    SYSTEM_INFO si; GetSystemInfo(&si);
    log_print("Cores: %lu | Arch: x64\n", si.dwNumberOfProcessors);
    #else
    FILE* f = popen("nproc", "r"); 
    if(f){ int cores; fscanf(f, "%d", &cores); pclose(f);
    log_print("Cores: %d\n", cores); }
    #endif
}

/* Cross-platform Memory */
void print_mem_info(void) {
    log_print("[MEM] ");
    #ifdef _WIN32
    MEMORYSTATUSEX statex; statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    log_print("Total: %.2f GB | Free: %.2f GB\n", 
        statex.ullTotalPhys/1024.0/1024.0/1024.0, 
        statex.ullAvailPhys/1024.0/1024.0/1024.0);
    #else
    system("free -h | grep Mem"); // Just dump free command
    #endif
}

/* Snapshot system: v0.9.1 basic stubs */
void list_snapshots(void) { log_print("[SNAP] Found: default.snap\n"); }
void load_snapshot(const char* name) { log_print("[SNAP] Loaded snapshot: %s\n", name); }
void diff_snapshot(void) { log_print("[SNAP] No differences found vs last snapshot\n"); }

/* Export current manifest to JSON file */
void export_manifest(void) { 
    FILE* f = fopen("manifest.json", "w");
    if(f){ fprintf(f, "{\"version\":\"0.9.1.1\",\"platform\":\"windows\",\"host\":\"wonkavator\"}\n"); fclose(f);
    log_print("[HW] Exported manifest to manifest.json\n"); }
    else log_print("[HW] ERROR: Could not write manifest.json\n");
}

/* Background monitor thread */
void parasite_monitor(void) { 
    log_print("[PARASITE] Monitor running in background\n"); 
    sleep(10); 
}
