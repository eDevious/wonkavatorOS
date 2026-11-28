bash
#!/bin/bash
set -e

echo "[SCAFFOLD] Building Wonkavator v0.9.1"
rm -rf ~/wonkavatorOS
mkdir -p ~/wonkavatorOS/stage0
cd ~/wonkavatorOS

# 1. Makefile
cat > Makefile << 'EOF'
CC=gcc
CFLAGS=-std=c17 -Wall -O2 -Istage0 -pthread
SRC=stage0/main.c stage0/cli.c stage0/hardware.c stage0/orchestrator.c stage0/net_scan.c stage0/hardware_fingerprint.c
OBJ=$(SRC:.c=.o)
TARGET=wonkavator

all: $(TARGET)
	@echo "[BUILD] Wonkavator v0.9.1 ready"

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -lpthread

clean:
	rm -f $(OBJ) $(TARGET)
	@echo "[CLEAN] Done"

.PHONY: all clean
EOF

# 2. boot.h
cat > stage0/boot.h << 'EOF'
#ifndef BOOT_H
#define BOOT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>

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

void quick_scan(void); void full_manifest(void); void probe_system(void);
void print_host_info(void); void print_cpu_info(void); void print_mem_info(void);
void list_snapshots(void); void load_snapshot(const char* name); void diff_snapshot(void);
void export_manifest(void); void parasite_monitor(void);
void orchestrator_tick(void); void orchestrator_status(void);
void network_herd_add(const char* ip, int port); void network_tick(void);
const char* pick_best_node(const char* metric); void qmp_send(const char* node, const char* cmd);
void adaptive_net_discovery(void); int is_host_up(const char* ip); void print_open_ports(const char* ip);
void cli_loop(void); void* watch_thread(void* arg); void* parasite_thread(void* arg);
void fingerprint_hardware(void); void fingerprint_microcontrollers(void); void fingerprint_firmware_remote(const char* ip);
static inline void tracer_log(const char* name, double ms) { (void)name; (void)ms; }
static inline void chip_detect(void) { log_print("[CHIP] Detection stub\n"); }
static inline void adaptive_boot(void) { log_print("[BOOT] Adaptive boot stub\n"); }

#endif
EOF

# 3. main.c
cat > stage0/main.c << 'EOF'
#include "boot.h"
int main(int argc, char** argv) {
    (void)argc; (void)argv;
    log_print("[WONKAVATOR] v0.9.1 STOPPABLE SCAN Booting...\n");
    cli_loop();
    log_print("[WONKAVATOR] Shutdown complete.\n");
    return 0;
}
EOF

# 4. cli.c - paste the full cli.c from last message here
cat > stage0/cli.c << 'EOF'
// PASTE FULL cli.c FROM PREVIOUS MESSAGE
EOF

# 5. hardware_fingerprint.c
cat > stage0/hardware_fingerprint.c << 'EOF'
// PASTE FULL hardware_fingerprint.c FROM PREVIOUS MESSAGE  
EOF

# 6. STUBS for the other 3 so it compiles
cat > stage0/net_scan.c << 'EOF'
#include "boot.h"
void adaptive_net_discovery(void){log_print("[NET] Adaptive scan stub\n");}
int is_host_up(const char* ip){(void)ip;return 0;}
EOF

cat > stage0/hardware.c << 'EOF'
#include "boot.h"
void quick_scan(void){log_print("[HW] Quick scan stub\n");}
void full_manifest(void){log_print("[HW] Full manifest stub\n");}
void probe_system(void){log_print("[HW] Probe stub\n");}
void print_host_info(void){log_print("[HW] Host info stub\n");}
void print_cpu_info(void){log_print("[HW] CPU info stub\n");}
void print_mem_info(void){log_print("[HW] Mem info stub\n");}
void list_snapshots(void){log_print("[HW] List snapshots stub\n");}
void load_snapshot(const char* name){log_print("[HW] Load %s stub\n", name);}
void diff_snapshot(void){log_print("[HW] Diff stub\n");}
void export_manifest(void){log_print("[HW] Export stub\n");}
void parasite_monitor(void){log_print("[PARASITE] Monitor tick\n");}
EOF

cat > stage0/orchestrator.c << 'EOF'
#include "boot.h"
void orchestrator_tick(void){log_print("[ORCH] Tick\n");}
void orchestrator_status(void){log_print("[ORCH] Herd status\n");}
void network_herd_add(const char* ip, int port){log_print("[ORCH] Add %s:%d\n", ip, port);}
void network_tick(void){log_print("[ORCH] Network tick\n");}
const char* pick_best_node(const char* metric){(void)metric; return "localhost";}
void qmp_send(const char* node, const char* cmd){log_print("[QMP] %s -> %s\n", node, cmd);}
EOF

echo "[SCAFFOLD] Done. Run: cd ~/wonkavatorOS && make"
