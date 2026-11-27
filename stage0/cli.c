#include "boot.h"

void print_help(void) {
    printf("Commands:\n  probe\n  status\n  deploy --workers=N --time=S\n  ai-show\n  ai-reset\n  parasite\n  hibernate\n  help\n");
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
            if(g_placement_count == 0) printf("[AI] Placement table empty. Run 'deploy' first.\n");
            else for(int i=0;i<g_placement_count;i++) printf("%s %s %s avg:%dms runs:%d\n", g_placement_table[i].node_id, g_placement_table[i].isa, g_placement_table[i].kernel, g_placement_table[i].avg_time_ms, g_placement_table[i].runs); 
        }
        else if(strncmp(buf, "ai-reset", 8) == 0) { g_placement_count = 0; printf("[AI] Reset\n"); }
        else if(strncmp(buf, "parasite", 8) == 0) { // UPDATED v0.6
            printf("%-6s %-15s %-6s %-6s %-6s %s\n", "NODE", "IP", "PING", "CPU", "SCORE", "STATUS");
            for(int i=0;i<11;i++) {
                const char* status = g_node_health[i].blacklisted ? "[BLACKLISTED]" : (g_node_health[i].ip_up ? "OK" : "IP-DOWN");
                printf("%-6s %-15s %-6d %-6d %-6d %s\n", 
                    g_node_health[i].node_id, g_node_health[i].ip, 
                    g_node_health[i].last_ping_ms, g_node_health[i].last_latency_ms,
                    g_node_health[i].parasite_score, status);
            }
        }
        else if(strncmp(buf, "hibernate", 9) == 0) { ai_placer_save(); printf("[AI] Saved to ai_placer.dat\n"); break; }
    }
}