#include "boot.h"

int main(int argc, char** argv) {
    (void)argc; (void)argv;
    log_print("[WONKAVATOR] v0.9.1 STOPPABLE SCAN Booting...\n");
    cli_loop();
    log_print("[WONKAVATOR] Shutdown complete.\n");
    return 0;
}
