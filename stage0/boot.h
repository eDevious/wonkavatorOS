#ifndef BOOT_H
#define BOOT_H

#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <pthread.h>

void log_print(const char* fmt, ...);

void print_host_info(void);
void print_cpu_info(void);
void print_mem_info(void);
void print_local_hardware_verbose(void);
void scan_pci_devices(void);
void scan_usb_devices(void);
void print_dmi_info(void);
void create_snapshot(void);
void diff_snapshot(void);
void full_manifest(void);
void export_manifest(void);

extern int g_watching;
void* watch_thread(void* arg);
void cli_loop(void);

#endif
