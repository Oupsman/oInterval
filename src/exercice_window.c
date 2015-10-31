#include <pebble.h>
#include "exercice_window.h"

static uint8_t number;
  
void init_workouts () {
  
  uint8_t i;
  
  All[0].run = 120;
  All[0].walk = 60;
  All[0].repeat = 10;
  All[1].run = 180;
  All[1].walk = 60;
  All[1].repeat = 10;
  All[2].run = 300;
  All[2].walk = 120;
  All[2].repeat = 10;
  All[3].run = 600;
  All[3].walk = 180;
  All[3].repeat = 10;
  All[4].run = 900;
  All[4].walk = 300;
  All[4].repeat = 3;
  
  for (i=0;i<5;i++) {
    APP_LOG (APP_LOG_LEVEL_INFO, "ALL[%u].run = %u", i, All[i].run);
  }
  
}

void update_screen (Layer *me, GContext *ctx) {
  
  uint8_t i;
  
  uint16_t total; 
  uint32_t start_angle_run, stop_angle_run, start_angle_walk; 
  
 //  graphics_draw_rect (ctx,GRect (0,0,144,144));
  total = time_run + time_walk;
  
  start_angle_run = 360*(time_run-seconds_to_go)/total;
  stop_angle_run = 360*time_run/total;
  if (is_running) {
    start_angle_walk = 360*time_run/total;
  } else {
    start_angle_walk = 360*(total-seconds_to_go)/total;
  }
  
  graphics_context_set_stroke_width(ctx, 5);
  
  graphics_context_set_antialiased(ctx, true);
  
  if (is_running) {

    graphics_context_set_stroke_color (ctx,GColorOrange);
    graphics_draw_arc (ctx, GRect (5,5,134,134), GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(start_angle_run), DEG_TO_TRIGANGLE(stop_angle_run));
    
  }
  
  graphics_context_set_stroke_color (ctx,GColorGreen);
  
  graphics_draw_arc (ctx, GRect (5,5,134,134), GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(start_angle_walk), DEG_TO_TRIGANGLE(360));
  
  graphics_context_set_stroke_color (ctx,GColorBlack);
  graphics_context_set_stroke_width (ctx, 2); 
  
  if (seconds_to_go % 2 == 0) {
    graphics_draw_arc (ctx, GRect (10,10,124,124), GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE (0), DEG_TO_TRIGANGLE(360*(current_cycle)/All[number].repeat));  
  } else {
    graphics_draw_arc (ctx, GRect (10,10,124,124), GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE (0), DEG_TO_TRIGANGLE(360*(current_cycle-1)/All[number].repeat));
  }
}

void start_workout (uint8_t which) {
  
  number = which;
  
  workout_window_init ();
 
}

void do_workout (struct tm *tick_time, TimeUnits units_changed) {
  
  APP_LOG(APP_LOG_LEVEL_INFO, "Seconds to go : %u", seconds_to_go);
  
  if (seconds_to_go>0) {

    minutes = seconds_to_go / 60;
    seconds = seconds_to_go % 60;
    snprintf (buffer,sizeof(buffer), "%u:%02u", minutes,seconds);
    text_layer_set_text (l_timetogo,buffer);
    
    snprintf (cycle,sizeof(cycle),"%u/%u", current_cycle,All[number].repeat);
    text_layer_set_text (l_repeat, cycle);
  
    seconds_to_go--;
      
  } else {
      
    vibes_long_pulse();
    switch (is_running) {
      case 0:
        is_running = 1;
        seconds_to_go = All[number].run;
        current_cycle++;
        break;
      case 1:
        is_running = 0;
        seconds_to_go = All[number].walk;
        break;
      
    }
  }
  layer_mark_dirty (s_funky_layer);
}

void workout_window_load () {
  
  l_timetogo = text_layer_create (GRect(30, 50, 80, 30));
  s_funky_layer = layer_create (GRect(0,16,144,144));
  l_repeat = text_layer_create (GRect(30,80,80,30));
  
  layer_set_update_proc (s_funky_layer,update_screen);
  
  window_set_background_color(s_workout_window, GColorWhite);
  text_layer_set_background_color(l_timetogo, GColorWhite);
  text_layer_set_text_color(l_timetogo, GColorDarkCandyAppleRed);
    text_layer_set_background_color(l_repeat, GColorWhite);
  text_layer_set_text_color(l_repeat, GColorDarkCandyAppleRed);
  
  text_layer_set_font(l_timetogo, fonts_get_system_font(FONT_KEY_LECO_26_BOLD_NUMBERS_AM_PM));
  text_layer_set_text_alignment(l_timetogo, GTextAlignmentCenter);
  text_layer_set_font(l_repeat, fonts_get_system_font(FONT_KEY_LECO_26_BOLD_NUMBERS_AM_PM));
  text_layer_set_text_alignment(l_repeat, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(s_workout_window), s_funky_layer); 
  layer_add_child(window_get_root_layer(s_workout_window), text_layer_get_layer(l_timetogo));
  layer_add_child(window_get_root_layer(s_workout_window), text_layer_get_layer(l_repeat));
  
  init_workouts () ;

  seconds_to_go = All[number].run;
  is_running = 1;
  current_cycle = 1;
  
  time_run = All[number].run;
  time_walk = All[number].walk;
  
  minutes = seconds_to_go / 60;
  seconds = seconds_to_go % 60;

  tick_timer_service_subscribe (SECOND_UNIT, &do_workout);
  
}

void workout_window_unload () {
  
  text_layer_destroy (l_timetogo);
  tick_timer_service_unsubscribe ();  
  window_destroy(s_workout_window);
  
}

void workout_window_init () {
    
  s_workout_window = window_create();
  WindowHandlers handlers = {
      .load = workout_window_load,
      .unload = workout_window_unload
  };
  
  window_set_window_handlers(s_workout_window, (WindowHandlers) handlers);
  window_stack_push(s_workout_window, true);  
  
}