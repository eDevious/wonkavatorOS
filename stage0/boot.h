#ifndef BOOT_H
#define BOOT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

void dma_init(void);
void ai_placer_load(void);
void ai_placer_save(void);
void orchestrator_run(int cores, int gpu);
void cli_loop(void);

void full_manifest(void);
void diff_snapshot(void);
void scan_host_system(void);
void scan_universal_controllers(void);
void scan_all_mcus(void);
void print_local_hardware_verbose(void);
void log_print(const char* fmt, ...);
void save_snapshot(void);
#endif
