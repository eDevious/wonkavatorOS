#include "boot.h"
#include <string.h>
void cli_loop(void) {
    char line[256];
    log_print("[WONKAVATOR] Running initial manifest...\n");
    full_manifest();
    log_print("[LEVIATHAN] Type 'help' for commands\n\n");
    while (1) {
        printf("[LEVIATHAN] > ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) break;
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, "help") == 0) {
            log_print("Commands: help, manifest, diff, exit\n");
        } else if (strcmp(line, "manifest") == 0) {
            create_snapshot();
        } else if (strcmp(line, "diff") == 0) {
            diff_snapshot();
        } else if (strcmp(line, "exit") == 0) {
            log_print("[LEVIATHAN] Shutting down.\n");
            break;
        } else if (strlen(line) > 0) {
            log_print("Unknown command: %s\n", line);
        }
    }
}
