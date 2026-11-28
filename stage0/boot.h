#ifndef BOOT_H
#define BOOT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ========== PLATFORM ABSTRACTION LAYER ========== */
#ifdef _WIN32
    #define _CRT_SECURE_NO_WARNINGS
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <winsock2.h>
    #include <iphlpapi.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "iphlpapi.lib")
    #define popen _popen
    #define pclose _pclose
    #define sleep(x) Sleep((x)*1000)
    #define PATH_SEP '\\'
    typedef SOCKET socket_t;
    typedef DWORD thread_t;
    #define THREAD_RETURN DWORD WINAPI
#else
    #include <unistd.h>
    #include <pthread.h>
    #include <stdarg.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #define PATH_SEP '/'
    typedef int socket_t;
    typedef pthread_t thread_t;
    #define THREAD_RETURN void*
#endif

/* Microcontroller stub - for future ESP32/STM32 port */
#ifdef __MICROCONTROLLER__
    #undef popen
    #undef pclose
    #define LOG_FILE "/sd/wonkavator.log" // SD card log
#else
    #define LOG_FILE "wonkavator.log"
#endif

/* ========== LOGGING ========== */
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

/* ========== HARDWARE MODULE ========== */
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

/* ========== ORCHESTRATOR MODULE ========== */
void orchestrator_tick(void); 
void orchestrator_status(void);
void network_herd_add(const char* ip, int port); 
void network_tick(void);
const char* pick_best_node(const char* metric); 
void qmp_send(const char* node, const char* cmd);

/* ========== NETWORK MODULE ========== */
void adaptive_net_discovery(void); 
int is_host_up(const char* ip); 
void print_open_ports(const char* ip);

/* ========== CLI MODULE ========== */
void cli_loop(void); 
THREAD_RETURN watch_thread(void* arg); 
THREAD_RETURN parasite_thread(void* arg);

/* ========== FINGERPRINT MODULE ========== */
void fingerprint_hardware(void); 
void fingerprint_microcontrollers(void); 
void fingerprint_firmware_remote(const char* ip);

/* ========== STUBS FOR FUTURE MODULES ========== */
static inline void tracer_log(const char* name, double ms) { (void)name; (void)ms; }
static inline void chip_detect(void) { log_print("[CHIP] Detection stub\n"); }
static inline void adaptive_boot(void) { log_print("[BOOT] Adaptive boot stub\n"); }

#endif
2. stage0/main.c
c
#include "boot.h"

/* 
 * WONKAVATOR v0.9.1 ENTRY POINT
 * Boot sequence: Init -> Spawn threads -> CLI Loop
 */
int main(int argc, char* argv[]) {
    log_print("=================================\n");
    log_print("  WONKAVATOR v0.9.1 BOOTING\n");
    log_print("  Platform: ");
    #ifdef _WIN32
    log_print("WINDOWS\n");
    #elif __linux__
    log_print("LINUX\n");
    #elif __MICROCONTROLLER__
    log_print("MICROCONTROLLER\n");
    #else
    log_print("UNKNOWN\n");
    #endif
    log_print("=================================\n");

    #ifdef _WIN32
    // Init Winsock for Windows network calls
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
    #endif

    // Spawn background threads
    thread_t t1, t2;
    #ifdef _WIN32
    t1 = CreateThread(NULL, 0, watch_thread, NULL, 0, NULL);
    t2 = CreateThread(NULL, 0, parasite_thread, NULL, 0, NULL);
    #else
    pthread_create(&t1, NULL, watch_thread, NULL);
    pthread_create(&t2, NULL, parasite_thread, NULL);
    #endif

    // Main CLI
    cli_loop();

    #ifdef _WIN32
    WSACleanup();
    #endif
    log_print("[BOOT] Shutdown complete\n");
    return 0;
}
3. stage0/cli.c
c
#include "boot.h"
#include <ctype.h>

/* LEVIATHAN COMMAND LOOP v0.9.1 */
void cli_loop(void) {
    char cmd[256];
    log_print("wonkavator> ");
    while (fgets(cmd, sizeof(cmd), stdin)) {
        cmd[strcspn(cmd, "\n")] = 0; // strip newline
        
        if (strcmp(cmd, "help") == 0) {
            log_print("Commands: scan, manifest, probe, net, orch, snap, export, stop\n");
        } else if (strcmp(cmd, "scan") == 0) {
            quick_scan();
        } else if (strcmp(cmd, "manifest") == 0) {
            full_manifest();
        } else if (strncmp(cmd, "probe", 5) == 0) {
            probe_system();
        } else if (strncmp(cmd, "net", 3) == 0) {
            adaptive_net_discovery();
        } else if (strcmp(cmd, "stop") == 0) {
            log_print("Stopping...\n");
            break;
        } else {
            log_print("Unknown command: %s\n", cmd);
        }
        log_print("wonkavator> ");
    }
}

/* Background watcher thread */
THREAD_RETURN watch_thread(void* arg) {
    (void)arg;
    while(1) {
        orchestrator_tick();
        sleep(5);
    }
    return 0;
}

/* Background parasite monitor */
THREAD_RETURN parasite_thread(void* arg) {
    (void)arg;
    parasite_monitor();
    return 0;
}
