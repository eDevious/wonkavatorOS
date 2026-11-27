#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WORKERS 16
#define PLACEMENT_TABLE_SIZE 64
#define SCAN_PORTS 10

typedef struct { char node_id[8]; char isa[8]; char kernel[16]; uint32_t avg_time_ms; int runs; } placement_entry_t;
typedef struct { char id[8]; int online; uint32_t cpu_mhz; } worker_t;

typedef struct { char node_id[8]; char ip[16]; int parasite_score; uint32_t last_latency_ms; uint32_t last_ping_ms; int blacklisted; int ip_up; int open_ports[SCAN_PORTS]; } node_health_t;

void cli_loop(void);
void orchestrator_run(int workers, int seconds);
void ai_placer_load(void);
void ai_placer_save(void);
int ai_placer_get_best_node(const char* isa, const char* kernel);
void ai_placer_update(const char* node, const char* isa, const char* kernel, uint32_t time_ms);
void dma_init(void);
void dma_memcpy(int ch, void* dst, void* src, size_t len);
void gpio_sync_pulse(void);

// UPDATED v0.6.1
void parasitic_scan(const char* target_ip);
int is_node_blacklisted(const char* node);
void parasitic_update(const char* node, uint32_t latency_ms);

extern worker_t g_workers[MAX_WORKERS];
extern placement_entry_t g_placement_table[PLACEMENT_TABLE_SIZE];
extern int g_placement_count;
extern node_health_t g_node_health[MAX_WORKERS];
