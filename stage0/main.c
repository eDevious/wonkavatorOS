#include "boot.h"

int main(int argc, char *argv[]) {
    printf("[WONKAVATOR] Forging WonkavatorOS v0.7 LEVIATHAN...\n");
    printf("[WONKAVATOR] Consolidated: v0.1-v0.7 features loaded.\n\n");
    cli_loop();
    printf("[WONKAVATOR] Shutdown complete.\n");
    return 0;
}
