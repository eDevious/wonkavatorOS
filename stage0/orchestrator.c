#include "boot.h"
#include <time.h>

void orchestrator_run(int workers, int seconds) {
    printf("[ORCH] Starting 80 job matrix on %d workers\n", workers);
    const char* isas[] = {"RV64", "ARM64", "x86_64"};
    const char* kernels[] = {"monolithic", "hybrid", "unikernel"};
    srand((unsigned)time(NULL));
    for(int j=0; j<80; j++) {
        const char* isa = isas[j%3]; const char* kernel = kernels[j%3];
        int node_idx = ai_placer_get_best_node(isa, kernel);
        uint32_t sim_time = 5 + (rand() % 10);
        printf("[AI-DISPATCH] %s %s -> worker-%d time:%dms\n", isa, kernel, node_idx, sim_time);
        ai_placer_update(g_workers[node_idx].id, isa, kernel, sim_time);
    }
    FILE* f = fopen("wonk.lock", "w"); fprintf(f, "[]"); fclose(f);
    printf("[ORCH] Done. Wrote wonk.lock\n");
}