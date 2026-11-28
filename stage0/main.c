#include "boot.h"

/* 
 * WONKAVATOR v0.9.1.1 ENTRY POINT
 * Boot order: Init Platform -> Spawn Threads -> Enter CLI
 */
int main(int argc, char* argv[]) {
    log_print("=================================\n");
    log_print("  WONKAVATOR v0.9.1.1 BOOTING\n");
    log_print("  Platform: ");
    #ifdef _WIN32
    log_print("WINDOWS x64\n");
    #elif __linux__
    log_print("LINUX\n");
    #else
    log_print("UNKNOWN\n");
    #endif
    log_print("=================================\n");

    #ifdef _WIN32
    // Must init Winsock before any socket/ping calls
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0){
        log_print("[FATAL] WSAStartup failed\n"); return 1;
    }
    #endif

    // Spawn background daemons
    thread_t t1, t2;
    #ifdef _WIN32
    t1 = CreateThread(NULL, 0, watch_thread, NULL, 0, NULL);
    t2 = CreateThread(NULL, 0, parasite_thread, NULL, 0, NULL);
    #else
    pthread_create(&t1, NULL, watch_thread, NULL);
    pthread_create(&t2, NULL, parasite_thread, NULL);
    #endif

    // Main thread becomes the CLI
    cli_loop();

    #ifdef _WIN32
    WSACleanup(); // Cleanup sockets
    #endif
    log_print("[BOOT] Shutdown complete\n");
    return 0;
