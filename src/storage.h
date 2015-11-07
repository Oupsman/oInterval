#pragma once
#include <pebble.h>

// Units : seconds
uint16_t custom_run, custom_walk;
uint8_t custom_repeat;

void load_datas (void);
void save_datas (void);
