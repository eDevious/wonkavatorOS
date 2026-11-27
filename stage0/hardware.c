#define _GNU_SOURCE
#include "boot.h"
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <stdarg.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>        // ONLY this one for ifreq
#include <sys/ioctl.h>     
#include <unistd.h>
#include <string.h>

#define SNAPSHOT_FILE "last_manifest.snap"
static FILE* g_logfile = NULL;
int g_watching = 0;

void log_print(const char* fmt, ...) {
    va_list args; va_start(args, fmt);
    vprintf(fmt, args);
    if(g_logfile) {
        va_list args2; va_copy(args2, args);
        vfprintf(g_logfile, fmt, args2); va_end(args2);
    }
    va_end(args);
}

void open_log(const char* prefix) {
    time_t t = time(NULL); struct tm tm = *localtime(&t);
    char filename[64];
    snprintf(filename, 64, "%s_%04d%02d%02d_%02d%02d%02d.txt", 
             prefix, tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    g_logfile = fopen(filename, "w");
    if(g_logfile) fprintf(g_logfile, "=== WONKAVATOR v0.7 LEVIATHAN LOG ===\nTimestamp: %s", ctime(&t));
    printf("[LEVIATHAN] LOGGING TO: %s\n", filename);
}
void close_log(void) { if(g_logfile) {fprintf(g_logfile, "=== END LOG ===\n"); fclose(g_logfile); g_logfile = NULL;} }

void save_snapshot(void) {
    FILE* f = fopen(SNAPSHOT_FILE, "w");
    if(!f) return;
    struct utsname u; uname(&u);
    fprintf(f, "HOST:%s\nARCH:%s\n", u.nodename, u.machine);
    system("ls /dev/ttyACM* /dev/ttyUSB* 2>/dev/null >> " SNAPSHOT_FILE);
    system("ip -o link show | awk -F': ' '{print \"IF:\"$2}' >> " SNAPSHOT_FILE);
    fclose(f);
}

void diff_snapshot(void) {
    printf("[LEVIATHAN] === MANIFEST DIFF ===\n");
    FILE* old = fopen(SNAPSHOT_FILE, "r");
    if(!old) { printf("[NEW] No previous snapshot. Run 'manifest' once first.\n"); return; }
    fclose(old);
    
    printf("[NEW MCUs]\n");
    system("comm -13 " SNAPSHOT_FILE " <(ls /dev/ttyACM* /dev/ttyUSB* 2>/dev/null | sort) 2>/dev/null || echo None");
    printf("[NEW NETWORK IFs]\n");
    system("comm -13 " SNAPSHOT_FILE " <(ip -o link show | awk -F': ' '{print \"IF:\"$2}' | sort) 2>/dev/null || echo None");
    printf("[LEVIATHAN] === END DIFF ===\n\n");
}

void* watch_thread(void* arg) {
    printf("[LEVIATHAN] WATCH MODE ACTIVE. Type 'stop' to exit watch.\n");
    g_watching = 1;
    save_snapshot(); // baseline
    while(g_watching) {
        sleep(3);
        system("diff " SNAPSHOT_FILE " <(echo HOST:$(uname -n); echo ARCH:$(uname -m); ls /dev/ttyACM* /dev/ttyUSB* 2>/dev/null; ip -o link show | awk -F': ' '{print \"IF:\"$2}') > /tmp/wonk_diff 2>&1");
        FILE* f = fopen("/tmp/wonk_diff", "r");
        if(f) {
            fseek(f, 0, SEEK_END);
            if(ftell(f) > 0) {
                printf("\a[ALERT] HARDWARE CHANGE DETECTED!\n");
                diff_snapshot();
                save_snapshot();
                open_log("hotplug");
                log_print("[HOTPLUG EVENT DETECTED]\n");
                close_log();
            }
            fclose(f);
        }
    }
    return NULL;
}

void scan_host_system(void) {
    log_print("\n[LEVIATHAN] === HOST SYSTEM SCAN ===\n");
    struct utsname u; uname(&u);
    log_print("[OS] %s %s %s\n[HOSTNAME] %s\n", u.sysname, u.release, u.machine, u.nodename);
    struct sysinfo si; sysinfo(&si);
    log_print("[UPTIME] %ldd %ldh\n[RAM] %luMB / %luMB free\n[CORES] %ld\n", 
        si.uptime/86400, (si.uptime%86400)/3600, si.totalram/1024/1024, si.freeram/1024/1024, sysconf(_SC_NPROCESSORS_ONLN));
    log_print("[DISKS]\n"); fflush(stdout); system("lsblk -o NAME,SIZE,TYPE,MOUNTPOINT 2>/dev/null");
    log_print("[LEVIATHAN] === END HOST SCAN ===\n\n");
}

void scan_universal_controllers(void) {
    log_print("[LEVIATHAN] === UNIVERSAL CONTROLLER SCAN ===\n");
    log_print("[ARCH] "); fflush(stdout); system("uname -m");
    log_print("[BOARD] "); fflush(stdout); system("cat /proc/device-tree/model 2>/dev/null || dmidecode -s system-product-name 2>/dev/null || echo Unknown");
    log_print("[PCI]\n"); fflush(stdout); system("lspci 2>/dev/null | head -10");
    log_print("[USB TREE]\n"); fflush(stdout); system("lsusb -t 2>/dev/null");
    log_print("[LEVIATHAN] === END CONTROLLER SCAN ===\n\n");
}

void scan_all_mcus(void) {
    log_print("[LEVIATHAN] === MCU/USB SCAN ===\n");
    fflush(stdout); system("ls -l /dev/ttyACM* /dev/ttyUSB* 2>/dev/null");
    fflush(stdout); system("dmesg | grep -i 'usb\\|cp210\\|ch340\\|ftdi\\|arduino\\|esp\\|stm\\|rp2' | tail -5");
    log_print("[LEVIATHAN] === END MCU SCAN ===\n\n");
}

void print_local_hardware_verbose(void) {
    log_print("[LEVIATHAN] === NETWORK MANIFEST ===\n");
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) { perror("getifaddrs"); return; }
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;
        
        if (ifa->ifa_addr->sa_family == AF_INET) {
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr, ip, INET_ADDRSTRLEN);
            
            int fd = socket(AF_INET, SOCK_DGRAM, 0);
            if (fd < 0) continue;
            
            struct ifreq ifr;
            memset(&ifr, 0, sizeof(ifr));
            strncpy(ifr.ifr_name, ifa->ifa_name, IF_NAMESIZE - 1);
            
            char mac_str[18] = "N/A";
            if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0) {
                // WSL2 fix: Skip if MAC is all zeros. That's WSL's fake interfaces
                unsigned char *mac = (unsigned char*)ifr.ifr_hwaddr.sa_data;
                if (mac[0] || mac[1] || mac[2] || mac[3] || mac[4] || mac[5]) {
                    snprintf(mac_str, sizeof(mac_str), "%02x:%02x:%02x:%02x:%02x:%02x", 
                        mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
                }
            }
            log_print("[IF] %s | IP: %s | MAC: %s\n", ifa->ifa_name, ip, mac_str);
            close(fd);
        }
    }
    freeifaddrs(ifaddr);
    log_print("[LEVIATHAN] === END NETWORK ===\n\n");
}

void full_manifest(void) {
    diff_snapshot();
    open_log("manifest");
    scan_host_system();
    scan_universal_controllers();
    scan_all_mcus();
    print_local_hardware_verbose();
    close_log();
    save_snapshot();
    printf("[LEVIATHAN] MANIFEST COMPLETE\n");
}
