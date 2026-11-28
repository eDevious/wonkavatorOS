#include "boot.h"
#include <stdio.h>
#include <string.h>

void run_cmd(const char* cmd) {
    FILE* fp = popen(cmd, "r");
    if (!fp) return;
    char buf[512];
    while (fgets(buf, sizeof(buf), fp)) {
        buf[strcspn(buf, "\n")] = 0;
        if (strlen(buf) > 0) log_print("  %s\n", buf);
    }
    pclose(fp);
}

void fingerprint_hardware(void) {
    log_print("[FINGERPRINT] === HARDWARE IDS ===\n");
    log_print("[FINGERPRINT] DMI/SMBIOS:\n");
    run_cmd("dmidecode -s system-manufacturer 2>/dev/null | sed 's/^/    Manufacturer: /'");
    run_cmd("dmidecode -s system-product-name 2>/dev/null | sed 's/^/    Product: /'");
    run_cmd("dmidecode -s system-serial-number 2>/dev/null | sed 's/^/    Serial: /'");
    run_cmd("dmidecode -s bios-version 2>/dev/null | sed 's/^/    BIOS: /'");
    
    log_print("[FINGERPRINT] PCI Devices:\n");
    run_cmd("lspci -nn | head -20");
    
    log_print("[FINGERPRINT] USB Devices:\n");
    run_cmd("lsusb");
    
    log_print("[FINGERPRINT] CPU:\n");
    run_cmd("grep 'model name' /proc/cpuinfo | head -1");
}

void fingerprint_microcontrollers(void) {
    log_print("[FINGERPRINT] === MICROCONTROLLERS / USB SERIAL ===\n");
    run_cmd("lsusb | grep -i 'arduino\\|ch340\\|cp210\\|ftdi\\|esp\\|stm32\\|pico'");
    log_print("[FINGERPRINT] Serial Ports:\n");
    run_cmd("ls -l /dev/ttyUSB* /dev/ttyACM* 2>/dev/null");
}

void fingerprint_firmware_remote(const char* ip) {
    log_print("[FINGERPRINT] === REMOTE PROBE: %s ===\n", ip);
    log_print("[FINGERPRINT] Banner grabbing...\n");
    print_open_ports(ip);
}
