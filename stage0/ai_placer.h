#pragma once
void ai_placer_load(void);
void ai_placer_save(void);
int ai_placer_get_best_node(const char* isa, const char* kernel);
void ai_placer_update(const char* node, const char* isa, const char* kernel, uint32_t time_ms);