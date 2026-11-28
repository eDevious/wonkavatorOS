#ifndef BOOT_H
#define BOOT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h> // For safe logging

/* ========== PLATFORM ABSTRACTION LAYER v0.9.1 ========== */
#ifdef _WIN32
    #define _CRT_SECURE_NO_WARNINGS
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <winsock2.h>
    #include <iphlpapi.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib") // Network sockets
    #pragma comment(lib, "iphlpapi.lib") // Network adapter info
    #define popen _popen // MSVC uses _popen
    #define pclose _pclose
    #define sleep(x) Sleep((x)*1000) // Sleep takes ms
    #define PATH_SEP '\\'
    typedef SOCKET socket_t;
    typedef DWORD thread_t;
    #define THREAD_RETURN DWORD WINAPI
#else // Linux / POSIX
    #include <unistd.h>
    #include <pthread.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #define PATH_SEP '/'
    typedef int socket_t;
    typedef pthread_t thread_t;
    #define THREAD_RETURN void*
#endif

/* Microcontroller path for future ESP32/STM32 port */
#ifdef __MICROCONTROLLER__
    #define LOG_FILE "/sd/wonkavator.log" // SD card logging
#else
    #define LOG_FILE "wonkavator.log" // Local file logging
#endif

/* ========== SAFE LOGGING v0.9.1.1 ========== */
/* Prints to console AND log file. Uses va_copy to prevent crash */
static inline void log_print(const char* fmt, ...) {
    va_list args1, args2;
    va_start(args1, fmt);
    va_copy(args2, args1); // Must copy before using twice

    vprintf(fmt, args1); // Console output
    
    FILE* f = fopen(LOG_FILE, "a"); // Append to log
    if (f) {
        vfprintf(f, fmt, args2); // File output
        fclose(f);
    }
    
    va_end(args1);
    va_end(args2);
}

/* ========== MODULE PROTOTYPES ========== */

/* hardware.c: Hardware scanning and snapshots */
void quick_scan(void);              // Fast CPU/RAM check
void full_manifest(void);           // Deep hardware enumeration
void probe_system(void);            // Sensor probe: temp, voltage
void print_host_info(void);         // Hostname, OS
void print_cpu_info(void);          // Core count, arch
void print_mem_info(void);          // Total/Free RAM
void list_snapshots(void);          // List saved .snap files
void load_snapshot(const char* name);// Load a .snap file
void diff_snapshot(void);           // Diff current vs last snap
void export_manifest(void);         // Dump hw to manifest.json
void parasite_monitor(void);        // Background resource monitor

/* orchestrator.c: Multi-node coordination */
void orchestrator_tick(void);                   // Called every 5s by thread
void orchestrator_status(void);                 // Print herd status
void network_herd_add(const char* ip, int port);// Add node to cluster
void network_tick(void);                        // Ping all nodes
const char* pick_best_node(const char* metric); // Scheduler: pick least loaded
void qmp_send(const char* node, const char* cmd);// Send QMP to remote node

/* net_scan.c: Network discovery */
void adaptive_net_discovery(void); // Scan 192.168.1.0/24
int is_host_up(const char* ip);    // Ping check
void print_open_ports(const char* ip); // TCP connect scan

/* cli.c: User interface */
void cli_loop(void);                        // Main REPL
THREAD_RETURN watch_thread(void* arg);      // Background orchestrator
THREAD_RETURN parasite_thread(void* arg);   // Background monitor

/* hardware_fingerprint.c: Device ID */
void fingerprint_hardware(void);                    // Local HW ID via WMI/dmidecode
void fingerprint_microcontrollers(void);            // USB scan for ESP/STM/Pico
void fingerprint_firmware_remote(const char* ip);   // SSH to remote and check fw

#endif
