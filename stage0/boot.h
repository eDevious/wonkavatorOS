#ifndef BOOT_H
#define BOOT_H

#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

void log_print(const char* fmt, ...);

// Core Scans
void print_host_info(void);
void print_cpu_info(void);
void print_mem_info(void);
void print_local_hardware_verbose(void);
void scan_pci_devices(void);
void scan_usb_devices(void);
void print_dmi_info(void);

// v0.1 - v0.6 Features
void create_snapshot(void);
void diff_snapshot(void);
void list_snapshots(void);
void load_snapshot(const char* fname);
void export_manifest(void);
void quick_scan(void);
void full_manifest(void);

// v0.7 New Features
void probe_system(void);
void parasite_monitor(void);
void tracer_log(const char* func, double ms);

// CLI
extern int g_watching;
extern int g_parasite;
void* watch_thread(void* arg);
void* parasite_thread(void* arg);
void cli_loop(void);

#endif
