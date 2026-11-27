#include "boot.h"
#include <unistd.h>
#include <pthread.h>

extern void* watch_thread(void* arg);
extern int g_watching;

void cli_loop(void) {
    char buf[256];
    pthread_t watcher;
    printf("wonk> ");
    while(fgets(buf, 256, stdin)) {
        buf[strcspn(buf, "\n")] = 0;
        
        if(strcmp(buf, "exit") == 0) { g_watching = 0; break; }
        else if(strcmp(buf, "manifest") == 0) { full_manifest(); }
        else if(strcmp(buf, "diff") == 0) { diff_snapshot(); }
        else if(strcmp(buf, "watch") == 0) { 
            if(!g_watching) pthread_create(&watcher, NULL, watch_thread, NULL);
            else printf("[WATCH] Already running\n");
        }
        else if(strcmp(buf, "stop") == 0) { g_watching = 0; printf("[WATCH] Stopped\n"); }
        else if(strcmp(buf, "host") == 0) { scan_host_system(); }
        else if(strcmp(buf, "board") == 0) { scan_universal_controllers(); }
        else if(strcmp(buf, "mcu") == 0) { scan_all_mcus(); }
        else if(strcmp(buf, "net") == 0) { print_local_hardware_verbose(); }
        else if(strcmp(buf, "help") == 0) {
            printf("Commands: manifest, diff, watch, stop, host, board, mcu, net, exit\n");
        }
        else { printf("Unknown command. Type 'help'\n"); }
        if(!g_watching) printf("wonk> ");
    }
}
