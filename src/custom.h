#pragma once
#include "storage.h"

Window *s_custom_window;
uint8_t field;
uint8_t max_fields = 5;

TextLayer *s_label_run;
TextLayer *s_label_walk;
TextLayer *s_label_repeat;
TextLayer *s_run_min;
TextLayer *s_run_sec;
TextLayer *s_walk_min;
TextLayer *s_walk_sec;
TextLayer *s_repeat;


void start_custom (void);
void custom_update_view (void);
