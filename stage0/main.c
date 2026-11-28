#include "boot.h"

/* 
 * WONKAVATOR v0.9.1 ENTRY POINT
 * Boot sequence: Init -> Spawn threads -> CLI Loop
 */
int main(int argc, char* argv[]) {
    log_print("=================================\n");
    log_print("  WONKAVATOR v0.9.1 BOOTING\n");
    log_print("  Platform: ");
    #ifdef _WIN32
    log_print("WINDOWS\n");
    #elif __linux__
    log_print("LINUX\n");
    #elif __MICROCONTROLLER__
    log_print("MICROCONTROLLER\n");
    #else
    log_print("UNKNOWN\n");
    #endif
    log_print("=================================\n");

    #ifdef _WIN32
    // Init Winsock for Windows network calls
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
    #endif

    // Spawn background threads
    thread_t t1, t2;
    #ifdef _WIN32
    t1 = CreateThread(NULL, 0, watch_thread, NULL, 0, NULL);
    t2 = CreateThread(NULL, 0, parasite_thread, NULL, 0, NULL);
    #else
    pthread_create(&t1, NULL, watch_thread, NULL);
    pthread_create(&t2, NULL, parasite_thread, NULL);
    #endif

    // Main CLI
    cli_loop();

    #ifdef _WIN32
    WSACleanup();
    #endif
    log_print("[BOOT] Shutdown complete\n");
    return 0;
}
