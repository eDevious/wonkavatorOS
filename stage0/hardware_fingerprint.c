#include "boot.h"

/* Generate unique HW ID for this machine */
void fingerprint_hardware(void) {
    log_print("[FP] Fingerprinting host hardware...\n");
    #ifdef _WIN32
    log_print("[FP] Win32: Will use WMI Win32_Processor + Win32_BaseBoard\n");
    #else
    log_print("[FP] Linux: Will use dmidecode -s system-uuid\n");
    #endif
}

/* Scan USB/Serial for dev boards */
void fingerprint_microcontrollers(void) {
    log_print("[FP] Scanning USB/Serial for ESP32/STM32/Pico...\n");
    log_print("[FP] TODO: Implement libusb enumeration\n");
}

/* SSH to remote and check firmware version */
void fingerprint_firmware_remote(const char* ip) {
    log_print("[FP] Remote firmware probe for %s: TODO\n", ip);
}
