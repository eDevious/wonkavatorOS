#define _GNU_SOURCE
#include "boot.h"
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <stdarg.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char last_snapshot[128] = {0};

void log_print(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void print_host_info(void) {
    struct utsname u;
    uname(&u);
    log_print("[LEVIATHAN] === HOST SYSTEM SCAN ===\n");
    log_print("[OS] %s %s %s\n", u.sysname, u.release, u.machine);
    log_print("[HOSTNAME] %s\n", u.nodename);
}

void print_cpu_info(void) {
    log_print("[LEVIATHAN] === CPU MANIFEST ===\n");
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (!fp) { log_print("[CPU] Cannot read /proc/cpuinfo\n"); return; }
    char buf[256];
    while (fgets(buf, sizeof(buf), fp)) {
        if (strncmp(buf, "model name", 10) == 0) {
            log_print("[CPU] %s", buf);
            break;
        }
    }
    fclose(fp);
    log_print("[LEVIATHAN] === END CPU ===\n\n");
}

void print_mem_info(void) {
    struct sysinfo si;
    sysinfo(&si);
    log_print("[LEVIATHAN] === MEMORY MANIFEST ===\n");
    log_print("[MEM] Total: %lu MB | Free: %lu MB | Used: %lu MB\n", 
        si.totalram / 1024 / 1024, si.freeram / 1024 / 1024, 
        (si.totalram - si.freeram) / 1024 / 1024);
    log_print("[LEVIATHAN] === END MEMORY ===\n\n");
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

void scan_pci_devices(void) { 
    log_print("[LEVIATHAN] === PCI MANIFEST ===\n"); 
    FILE *fp = popen("lspci -nn 2>/dev/null", "r"); 
    if (!fp) { log_print("[PCI] lspci not found or no permission\n"); return; } 
    char buf[512]; 
    while (fgets(buf, sizeof(buf), fp)) log_print("[PCI] %s", buf); 
    if (pclose(fp) != 0) log_print("[PCI] Scan failed. Run as root for full info.\n"); 
    log_print("[LEVIATHAN] === END PCI ===\n\n"); 
} 

void scan_usb_devices(void) { 
    log_print("[LEVIATHAN] === USB MANIFEST ===\n"); 
    FILE *fp = popen("lsusb 2>/dev/null", "r"); 
    if (!fp) { log_print("[USB] lsusb not found\n"); return; } 
    char buf[512]; 
    while (fgets(buf, sizeof(buf), fp)) log_print("[USB] %s", buf); 
    pclose(fp); 
    log_print("[LEVIATHAN] === END USB ===\n\n"); 
} 

void print_dmi_info(void) { 
    log_print("[LEVIATHAN] === DMI MANIFEST ===\n"); 
    FILE *fp = popen("dmidecode -s system-product-name 2>/dev/null", "r"); 
    if (!fp) { log_print("[DMI] dmidecode not found or run as root\n"); return; } 
    char buf[256]; 
    if (fgets(buf, sizeof(buf), fp)) log_print("[DMI] Product: %s", buf); 
    else log_print("[DMI] No DMI data. WSL2/VM detected.\n"); 
    pclose(fp); 
    log_print("[LEVIATHAN] === END DMI ===\n\n"); 
}

void create_snapshot(void) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(last_snapshot, sizeof(last_snapshot), "manifest_%Y%m%d_%H%M%S.txt", tm);
    FILE *fp = fopen(last_snapshot, "w");
    if (!fp) { perror("fopen"); return; }
    fprintf(fp, "[MANIFEST] %s", ctime(&t));
    fprintf(fp, "[STATUS] Snapshot created\n");
    fclose(fp);
    log_print("[LEVIATHAN] SNAPSHOT SAVED: %s\n", last_snapshot);
}

void diff_snapshot(void) {
    log_print("[LEVIATHAN] === MANIFEST DIFF ===\n");
    if (strlen(last_snapshot) == 0) {
        log_print("[NEW] No previous snapshot. Run 'manifest' once first.\n");
    } else {
        log_print("[DIFF] Comparing to: %s\n", last_snapshot);
        log_print("[DIFF] Feature coming in v0.7.1 - basic diff stub\n");
    }
    log_print("[LEVIATHAN] === END DIFF ===\n\n");
}

void export_manifest(void) {
    char csvname[128];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(csvname, sizeof(csvname), "manifest_%Y%m%d_%H%M%S.csv", tm);
    FILE *fp = fopen(csvname, "w");
    if (!fp) { perror("fopen"); return; }
    fprintf(fp, "Type,Name,Value\n");
    fprintf(fp, "HOST,OS,Linux\n");
    fprintf(fp, "HOST,Time,%s", ctime(&t));
    fclose(fp);
    log_print("[LEVIATHAN] EXPORTED: %s\n", csvname);
}

void full_manifest(void) {
    print_host_info();
    print_cpu_info();
    print_mem_info();
    print_local_hardware_verbose();
    scan_pci_devices();
    scan_usb_devices();
    print_dmi_info();
    create_snapshot();
}
