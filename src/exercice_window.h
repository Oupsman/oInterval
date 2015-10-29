#pragma once

typedef struct  {
  uint8_t run;
  uint8_t walk;
  uint8_t repeat;
} workout; 

workout All[5]; 

Window       *s_workout_window;
TextLayer    *l_timetogo;
TextLayer    *l_mode;
TextLayer    *l_repeat;
Layer        *s_funky_layer;

bool is_running;
uint8_t current_cycle;
uint16_t seconds_to_go;

uint16_t time_run,time_walk;

uint8_t minutes, seconds;
char buffer[] = "00:00";
char cycle[] = "10/10";

void init_workouts ();

void start_workout (uint8_t);
void update_screen (Layer *, GContext *);
void workout_window_load ();
void workout_window_unload ();
void workout_window_init();
void do_workout(struct tm *, TimeUnits );