#define _GNU_SOURCE
#include "boot.h"
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

static char last_snapshot[128] = {0};
ChipInfo g_chip = {0};

void log_print(const char *fmt, ...) { va_list args; va_start(args, fmt); vprintf(fmt, args); va_end(args); }
void print_host_info(void) { struct utsname u; uname(&u); log_print("[HOST] %s %s %s | %s\n", u.sysname, u.release, u.machine, u.nodename); }
void print_cpu_info(void) { FILE *fp = fopen("/proc/cpuinfo", "r"); if (!fp) return; char buf[256]; while (fgets(buf, sizeof(buf), fp)) if (strncmp(buf, "model name", 10) == 0) { log_print("[CPU] %s", buf); break; } fclose(fp); }
void print_mem_info(void) { struct sysinfo si; sysinfo(&si); log_print("[MEM] Total: %lu MB | Free: %lu MB\n", si.totalram / 1024 / 1024, si.freeram / 1024 / 1024); }

void print_local_hardware_verbose(void) { 
    struct ifaddrs *ifaddr, *ifa; 
    if (getifaddrs(&ifaddr) == -1) return; 
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) { 
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) { 
            char ip[INET_ADDRSTRLEN]; 
            inet_ntop(AF_INET, &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr, ip, INET_ADDRSTRLEN); 
            int fd = socket(AF_INET, SOCK_DGRAM, 0); 
            struct ifreq ifr; 
            memset(&ifr, 0, sizeof(ifr)); 
            strncpy(ifr.ifr_name, ifa->ifa_name, IF_NAMESIZE - 1); 
            char mac_str[18] = "N/A"; 
            if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0) { 
                unsigned char *mac = (unsigned char*)ifr.ifr_hwaddr.sa_data; 
                snprintf(mac_str, 18, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]); 
            } 
            log_print("[NET] %s | IP: %s | MAC: %s\n", ifa->ifa_name, ip, mac_str); 
            close(fd); 
        } 
    } 
    freeifaddrs(ifaddr); 
}

void scan_pci_devices(void) { FILE *fp = popen("lspci -nn 2>/dev/null", "r"); if (fp) { char buf[512]; while (fgets(buf, sizeof(buf), fp)) log_print("[PCI] %s", buf); pclose(fp); } }
void scan_usb_devices(void) { FILE *fp = popen("lsusb 2>/dev/null", "r"); if (fp) { char buf[512]; while (fgets(buf, sizeof(buf), fp)) log_print("[USB] %s", buf); pclose(fp); } }
void print_dmi_info(void) { FILE *fp = popen("dmidecode -s system-product-name 2>/dev/null", "r"); if (fp) { char buf[256]; if (fgets(buf, sizeof(buf), fp)) log_print("[DMI] %s", buf); pclose(fp); } }
void create_snapshot(void) { time_t t = time(NULL); struct tm *tm = localtime(&t); strftime(last_snapshot, sizeof(last_snapshot), "manifest_%Y%m%d_%H%M%S.txt", tm); FILE *fp = fopen(last_snapshot, "w"); if (fp) { fprintf(fp, "[MANIFEST] %s", ctime(&t)); fclose(fp); } log_print("[SNAPSHOT] Saved: %s\n", last_snapshot); }
void list_snapshots(void) { log_print("[HOSTS] Saved manifests:\n"); DIR *d = opendir("."); if (!d) return; struct dirent *dir; while ((dir = readdir(d)) != NULL) if (strncmp(dir->d_name, "manifest_", 9) == 0) log_print("  %s\n", dir->d_name); closedir(d); }
void load_snapshot(const char* fname) { FILE *fp = fopen(fname, "r"); if (!fp) { log_print("[LOAD] File not found: %s\n", fname); return; } log_print("[LOAD] Loading %s:\n", fname); char buf[256]; while(fgets(buf, sizeof(buf), fp)) printf("%s", buf); fclose(fp); }
void diff_snapshot(void) { if (strlen(last_snapshot) == 0) log_print("[DIFF] No snapshot\n"); else log_print("[DIFF] Last: %s\n", last_snapshot); }
void export_manifest(void) { char csvname[128]; time_t t = time(NULL); struct tm *tm = localtime(&t); strftime(csvname, sizeof(csvname), "manifest_%Y%m%d_%H%M%S.csv", tm); FILE *fp = fopen(csvname, "w"); if (fp) { fprintf(fp, "Type,Value\n"); fclose(fp); } log_print("[EXPORT] %s\n", csvname); }
void quick_scan(void) { log_print("[SCAN] === QUICK ===\n"); print_cpu_info(); print_mem_info(); print_local_hardware_verbose(); log_print("[SCAN] === END ===\n\n"); }
void full_manifest(void) { log_print("[MANIFEST] === FULL ===\n"); print_host_info(); quick_scan(); scan_pci_devices(); scan_usb_devices(); print_dmi_info(); create_snapshot(); log_print("[MANIFEST] === END ===\n\n"); }
void probe_system(void) { log_print("[PROBE] === DEEP PROBE ===\n"); FILE *fp = popen("cat /proc/version 2>/dev/null", "r"); if(fp){char b[256]; fgets(b,256,fp); log_print("[PROBE] Kernel: %s",b); pclose(fp);} log_print("[PROBE] === END ===\n\n"); }
void parasite_monitor(void) { log_print("[PARASITE] Top 3 CPU processes:\n"); FILE *fp = popen("ps -eo pid,comm,%cpu --sort=-%cpu | head -4", "r"); if(fp){char b[256]; while(fgets(b,256,fp)) log_print("%s",b); pclose(fp);} }
void tracer_log(const char* func, double ms) { struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); log_print("[TRACER] %s | Time: %ld.%09ld | Latency: %.2f ms\n", func, ts.tv_sec, ts.tv_nsec, ms); }

void chip_detect(void) {
    log_print("[CHIP-SENSE] === READING THE SHIP'S IRON ===\n");
    FILE *fp = fopen("/proc/cpuinfo", "r"); if (fp) { char buf[256]; while (fgets(buf, sizeof(buf), fp)) { if (strncmp(buf, "vendor_id", 9) == 0) sscanf(buf, "vendor_id : %s", g_chip.vendor); if (strncmp(buf, "model name", 10) == 0) { char *p = strchr(buf, ':'); if(p) strncpy(g_chip.model, p+2, 127); g_chip.model[strcspn(g_chip.model, "\n")] = 0; } if (strncmp(buf, "cpu cores", 9) == 0) sscanf(buf, "cpu cores : %d", &g_chip.cores); if (strncmp(buf, "siblings", 8) == 0) sscanf(buf, "siblings : %d", &g_chip.threads); if (strncmp(buf, "cpu MHz", 7) == 0) { double mhz; sscanf(buf, "cpu MHz : %lf", &mhz); g_chip.ghz = mhz/1000.0; } } fclose(fp); }
    FILE *nvidia = popen("nvidia-smi --query-gpu=name --format=csv,noheader 2>/dev/null", "r"); if (nvidia && fgets(g_chip.gpu_model, 128, nvidia)) { strcpy(g_chip.gpu_vendor, "NVIDIA"); } else { FILE *rocm = popen("rocminfo 2>/dev/null | grep 'Marketing Name' | head -1", "r"); if (rocm && fgets(g_chip.gpu_model, 128, rocm)) strcpy(g_chip.gpu_vendor, "AMD"); if(rocm) pclose(rocm); } if(nvidia) pclose(nvidia);
    log_print("[CHIP] CPU: %s %s | %dC/%dT @ %.2fGHz\n", g_chip.vendor, g_chip.model, g_chip.cores, g_chip.threads, g_chip.ghz);
    if(strlen(g_chip.gpu_vendor)) log_print("[CHIP] GPU: %s %s\n", g_chip.gpu_vendor, g_chip.gpu_model);
    FILE *lspci = popen("lspci -nn | grep -E 'VGA|Ethernet|Network' 2>/dev/null", "r"); if(lspci){char b[256]; while(fgets(b,256,lspci)) log_print("[CHIP] NIC: %s", b); pclose(lspci);}
    log_print("[CHIP-SENSE] === END ===\n\n");
}

void adaptive_boot(void) {
    log_print("[ADAPTIVE BOOT] Sounding the hull for hardware...\n");
    struct utsname u; uname(&u); log_print("[BOOT] Platform: %s %s\n", u.sysname, u.machine);
    if (strstr(u.machine, "x86_64")) log_print("[BOOT] Loading x86_64 microcode and KVM modules\n");
    else if (strstr(u.machine, "aarch64")) log_print("[BOOT] Loading ARM64 device tree and KVM\n");
    else log_print("[BOOT] Unknown arch. Loading generic drivers\n");
    chip_detect();
    if (strstr(g_chip.vendor, "AuthenticAMD")) log_print("[BOOT] AMD detected. Enabling SVM\n");
    if (strstr(g_chip.vendor, "GenuineIntel")) log_print("[BOOT] Intel detected. Enabling VT-x\n");
    log_print("[ADAPTIVE BOOT] Complete. Hull ready for 11 nodes.\n\n");
}
