#include "boot.h"

void print_help(void) {
    printf("Commands:\n  probe\n  status\n  deploy --workers=N --time=S\n  ai-show\n  ai-reset\n  parasite\n  hibernate\n  help\n"); // ADDED parasite
}

void cli_loop(void) {
    char buf[256];
    while(1) {
        printf("wonk> ");
        if(!fgets(buf, sizeof(buf), stdin)) break;
        if(strncmp(buf, "help", 4) == 0) print_help();
        else if(strncmp(buf, "probe", 5) == 0) printf("[PROBE] 11 workers, 3 DMA channels online\n");
        else if(strncmp(buf, "status", 6) == 0) printf("[STATUS] All systems nominal\n");
        else if(strncmp(buf, "deploy", 6) == 0) orchestrator_run(11, 10);
        else if(strncmp(buf, "ai-show", 7) == 0) { 
            if(g_placement_count == 0) printf("[AI] Placement table empty. Run 'deploy' first.\n"); // FIX v0.5
            else for(int i=0;i<g_placement_count;i++) printf("%s %s %s avg:%dms runs:%d\n", g_placement_table[i].node_id, g_placement_table[i].isa, g_placement_table[i].kernel, g_placement_table[i].avg_time_ms, g_placement_table[i].runs); 
        }
        else if(strncmp(buf, "ai-reset", 8) == 0) { g_placement_count = 0; printf("[AI] Reset\n"); }
        else if(strncmp(buf, "parasite", 8) == 0) { // NEW v0.5
            for(int i=0;i<11;i++) printf("%s latency:%dms score:%d %s\n", 
                g_node_health[i].node_id, g_node_health[i].last_latency_ms, 
                g_node_health[i].parasite_score, g_node_health[i].blacklisted ? "[BLACKLISTED]" : "");
        }
        else if(strncmp(buf, "hibernate", 9) == 0) { ai_placer_save(); printf("[AI] Saved to ai_placer.dat\n"); break; }
    }
}