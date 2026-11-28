#ifndef BOOT_H
#define BOOT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #define _CRT_SECURE_NO_WARNINGS
    #include <windows.h>
    #include <winsock2.h>
    #include <iphlpapi.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "iphlpapi.lib")
    #define popen _popen
    #define pclose _pclose
    #define sleep(x) Sleep((x)*1000)
    typedef SOCKET socket_t;
#else
    #include <unistd.h>
    #include <pthread.h>
    #include <stdarg.h>
    typedef int socket_t;
#endif

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

// HW
void quick_scan(void); void full_manifest(void); void probe_system(void);
void print_host_info(void); void print_cpu_info(void); void print_mem_info(void);
void list_snapshots(void); void load_snapshot(const char* name); void diff_snapshot(void);
void export_manifest(void); void parasite_monitor(void);

// ORCH
void orchestrator_tick(void); void orchestrator_status(void);
void network_herd_add(const char* ip, int port); void network_tick(void);
const char* pick_best_node(const char* metric); void qmp_send(const char* node, const char* cmd);

// NET
void adaptive_net_discovery(void); int is_host_up(const char* ip); void print_open_ports(const char* ip);

// CLI
void cli_loop(void); 
#ifdef _WIN32
DWORD WINAPI watch_thread(LPVOID arg); DWORD WINAPI parasite_thread(LPVOID arg);
#else
void* watch_thread(void* arg); void* parasite_thread(void* arg);
#endif

// FINGERPRINT
void fingerprint_hardware(void); void fingerprint_microcontrollers(void); void fingerprint_firmware_remote(const char* ip);

// STUBS
static inline void tracer_log(const char* name, double ms) { (void)name; (void)ms; }
static inline void chip_detect(void) { log_print("[CHIP] Detection stub\n"); }
static inline void adaptive_boot(void) { log_print("[BOOT] Adaptive boot stub\n"); }

#endif
