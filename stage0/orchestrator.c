#include "boot.h"
#include <time.h>

void orchestrator_run(int workers, int seconds) {
    printf("[ORCH] Starting 80 job matrix on %d workers\n", workers);
    parasitic_scan(); // NEW v0.5
    const char* isas[] = {"RV64", "ARM64", "x86_64"};
    const char* kernels[] = {"monolithic", "hybrid", "unikernel"};
    srand((unsigned)time(NULL));
    for(int j=0; j<80; j++) {
        const char* isa = isas[j%3]; const char* kernel = kernels[j%3];
        int node_idx = ai_placer_get_best_node(isa, kernel);
        
        // PARASITIC BYPASS: skip blacklisted nodes // NEW v0.5
        int attempts = 0;
        while(is_node_blacklisted(g_workers[node_idx].id) && attempts < 11) {
            node_idx = (node_idx + 1) % 11;
            attempts++;
        }
        
        uint32_t sim_time = 5 + (rand() % 10);
        printf("[AI-DISPATCH] %s %s -> %s time:%dms\n", isa, kernel, g_workers[node_idx].id, sim_time);
        ai_placer_update(g_workers[node_idx].id, isa, kernel, sim_time);
        parasitic_update(g_workers[node_idx].id, sim_time); // NEW v0.5
    }
    FILE* f = fopen("wonk.lock", "w"); fprintf(f, "[]"); fclose(f);
    printf("[ORCH] Done. Wrote wonk.lock\n");
}