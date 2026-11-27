#include "boot.h"

void ai_placer_load(void) {
    FILE* f = fopen("ai_placer.dat", "rb");
    if(f) { fread(&g_placement_count, sizeof(int), 1, f); fread(g_placement_table, sizeof(placement_entry_t), g_placement_count, f); fclose(f); printf("[AI] Loaded %d entries\n", g_placement_count); }
    else { printf("[AI] No previous placement data\n"); }
}

void ai_placer_save(void) {
    FILE* f = fopen("ai_placer.dat", "wb");
    if(f) { fwrite(&g_placement_count, sizeof(int), 1, f); fwrite(g_placement_table, sizeof(placement_entry_t), g_placement_count, f); fclose(f); }
}

int ai_placer_get_best_node(const char* isa, const char* kernel) {
    uint32_t best_time = 0xFFFFFFFF; int best_idx = 0;
    for(int i=0; i<g_placement_count; i++) {
        if(strcmp(g_placement_table[i].isa, isa) == 0 && strcmp(g_placement_table[i].kernel, kernel) == 0) {
            if(g_placement_table[i].avg_time_ms < best_time) { best_time = g_placement_table[i].avg_time_ms; best_idx = i; }
        }
    }
    return best_idx % 11;
}

void ai_placer_update(const char* node, const char* isa, const char* kernel, uint32_t time_ms) {
    for(int i=0; i<g_placement_count; i++) {
        if(strcmp(g_placement_table[i].node_id, node) == 0 && strcmp(g_placement_table[i].isa, isa) == 0) {
            g_placement_table[i].avg_time_ms = (g_placement_table[i].avg_time_ms * g_placement_table[i].runs + time_ms) / (g_placement_table[i].runs + 1);
            g_placement_table[i].runs++; return;
        }
    }
    if(g_placement_count < PLACEMENT_TABLE_SIZE) {
        strcpy(g_placement_table[g_placement_count].node_id, node);
        strcpy(g_placement_table[g_placement_count].isa, isa);
        strcpy(g_placement_table[g_placement_count].kernel, kernel);
        g_placement_table[g_placement_count].avg_time_ms = time_ms;
        g_placement_table[g_placement_count].runs = 1;
        g_placement_count++;
    }
}