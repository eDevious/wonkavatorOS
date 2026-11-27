#include "boot.h"

int main(int argc, char *argv[]) {
    printf("[WONKAVATOR] Forging WonkavatorOS v0.7 LEVIATHAN...\n");
    printf("[WONKAVATOR] All systems consolidated. Booting CLI.\n\n");
    cli_loop();
    printf("[WONKAVATOR] Shutdown complete.\n");
    return 0;
}
