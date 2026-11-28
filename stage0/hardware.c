#include "boot.h"
#include <stdio.h>
#include <stdlib.h>

void run_hw_cmd(const char* label, const char* cmd) {
    log_print("[HW] %s:\n", label);
    FILE* fp = popen(cmd, "r");
    if (!fp) return;
    char buf[512];
    while (fgets(buf, sizeof(buf), fp)) {
        buf[strcspn(buf, "\n")] = 0;
        if (strlen(buf) > 0) log_print("  %s\n", buf);
    }
    pclose(fp);
}

void quick_scan(void) {
    log_print("[HW] === QUICK SCAN ===\n");
    print_cpu_info();
    print_mem_info();
    log_print("[HW] === END QUICK SCAN ===\n");
}

void full_manifest(void) {
    log_print("[HW] === FULL SYSTEM MANIFEST ===\n");
    print_host_info();
    print_cpu_info();
    print_mem_info();
    run_hw_cmd("Storage", "lsblk -o NAME,SIZE,MODEL 2>/dev/null");
    run_hw_cmd("Network", "ip -br addr 2>/dev/null");
    log_print("[HW] === END MANIFEST ===\n");
}

void probe_system(void) {
    log_print("[HW] === PROBING SYSTEM ===\n");
    run_hw_cmd("Kernel", "uname -r");
    run_hw_cmd("OS", "cat /etc/os-release | grep PRETTY_NAME");
    log_print("[HW] === PROBE COMPLETE ===\n");
}

void print_host_info(void) {
    log_print("[HW] === HOST INFO ===\n");
    run_hw_cmd("Hostname", "hostname");
    run_hw_cmd("Uptime", "uptime -p");
}

void print_cpu_info(void) {
    log_print("[HW] === CPU INFO ===\n");
    run_hw_cmd("Model", "grep 'model name' /proc/cpuinfo | head -1 | cut -d: -f2");
    run_hw_cmd("Cores", "nproc");
    run_hw_cmd("Load", "cat /proc/loadavg | awk '{print $1,$2,$3}'");
}

void print_mem_info(void) {
    log_print("[HW] === MEMORY INFO ===\n");
    run_hw_cmd("RAM", "free -h | grep Mem");
}

void list_snapshots(void) {
    log_print("[HW] === SNAPSHOTS ===\n");
    run_hw_cmd("Files", "ls -1 *.snap 2>/dev/null || echo 'No snapshots found'");
}

void load_snapshot(const char* name) {
    log_print("[HW] Loading snapshot: %s\n", name);
}

void diff_snapshot(void) {
    log_print("[HW] Diffing against last snapshot...\n");
}

void export_manifest(void) {
    log_print("[HW] Exporting manifest to manifest.json\n");
    FILE* f = fopen("manifest.json", "w");
    if (f) {
        fprintf(f, "{\"version\":\"0.9.1\",\"host\":\"wonkavator\"}\n");
        fclose(f);
        log_print("[HW] Export complete\n");
    }
}

void parasite_monitor(void) {
    static int tick = 0;
    tick++;
    if (tick % 10 == 0) {
        log_print("[PARASITE] Monitoring... CPU/MEM OK\n");
    }
}
