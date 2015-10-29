#include <pebble.h>
#include "exercice_window.h"

static uint8_t number;
  
void init_workouts () {
  
  uint8_t i;
  
  All[0].run = 2;
  All[0].walk = 1;
  All[0].repeat = 10;
  All[1].run = 3;
  All[1].walk = 1;
  All[1].repeat = 10;
  All[2].run = 5;
  All[2].walk = 2;
  All[2].repeat = 10;
  All[3].run = 10;
  All[3].walk = 3;
  All[3].repeat = 10;
  All[4].run = 15;
  All[4].walk = 5;
  All[4].repeat = 3;
  
  for (i=0;i<5;i++) {
    APP_LOG (APP_LOG_LEVEL_INFO, "ALL[%u].run = %u", i, All[i].run);
  }
  
}
/*
void update_screen (Layer *me, GContext *ctx) {
  
  uint16_t total; 
  
  total = time_run + time_walk;
  
  graphics_context_set_stroke_width(ctx, 5);
  
  graphics_context_set_antialiased(ctx, true);
  
  graphics_context_set_stroke_color (ctx,GColorOrange);
  
  graphics_draw_arc (ctx, GRect (0,16,144,144), 1, 360* (total-seconds_to_go)/total, 360 * time_run / total);
  
  graphics_context_set_stroke_color (ctx,GColorGreen);
  
  graphics_draw_arc (ctx, GRect (0,16,144,144), 1, 360 * time_run / total, 360);
  
  
}*/

void start_workout (uint8_t which) {
  
  APP_LOG (APP_LOG_LEVEL_INFO, "Workout number %u", which); 
  
  number = which;
  
  workout_window_init ();
 
}

void do_workout (struct tm *tick_time, TimeUnits units_changed) {
  
  
  
  
    if (seconds_to_go>0) {
      APP_LOG (APP_LOG_LEVEL_INFO, "Doing a workout : %u seconds to go %u/%u cycles, %u:%02u to go", seconds_to_go, current_cycle, All[number].repeat, minutes, seconds);

      minutes = seconds_to_go / 60;
      seconds = seconds_to_go % 60;
      snprintf (buffer,sizeof(buffer), "%u:%02u", minutes,seconds);
      text_layer_set_text (l_timetogo,buffer);
      if (is_running) {
        text_layer_set_text (l_mode,"RUN !");
        
      } else {
        text_layer_set_text (l_mode,"WALK !");
          
      }
      
      snprintf (cycle,sizeof(cycle),"%u/%u", current_cycle,All[number].repeat);
      text_layer_set_text (l_repeat, cycle);
      
      seconds_to_go--;
      APP_LOG (APP_LOG_LEVEL_INFO, "Doing a workout : %u seconds to go %s cycles, %u:%u to go", seconds_to_go, cycle, minutes, seconds);
      
    } else {
      
      vibes_long_pulse();
      switch (is_running) {
        case 0:
          is_running = 1;
          seconds_to_go = All[number].run*60;
          current_cycle++;
        break;
        case 1:
          is_running = 0;
          seconds_to_go = All[number].walk*60;
        break;
      
      }
    }
  
}

void workout_window_load () {
  
  l_timetogo = text_layer_create (GRect(0, 16, 144, 44));
  l_mode = text_layer_create (GRect(0,60,144,44));
  l_repeat = text_layer_create (GRect(0,104,144,44)); 
  s_funky_layer = layer_create (GRect(0,16,144,144));
  
  // layer_set_update_proc (s_funky_layer,update_screen);
  
  window_set_background_color(s_workout_window, GColorWhite);
  text_layer_set_background_color(l_timetogo, GColorWhite);
  text_layer_set_text_color(l_timetogo, GColorBlack);
  text_layer_set_background_color(l_mode, GColorWhite);
  text_layer_set_text_color(l_mode, GColorBlack);
  text_layer_set_background_color(l_repeat, GColorWhite);
  text_layer_set_text_color(l_repeat, GColorBlack);
  
  text_layer_set_font(l_timetogo, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(l_timetogo, GTextAlignmentCenter);
  
  text_layer_set_font(l_mode, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(l_mode, GTextAlignmentCenter);
  text_layer_set_font(l_repeat, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(l_repeat, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(s_workout_window), s_funky_layer); 
  layer_add_child(window_get_root_layer(s_workout_window), text_layer_get_layer(l_timetogo));
  layer_add_child(window_get_root_layer(s_workout_window), text_layer_get_layer(l_mode));
  layer_add_child(window_get_root_layer(s_workout_window), text_layer_get_layer(l_repeat));
 
  
  text_layer_set_text (l_timetogo, "02:00");
  text_layer_set_text (l_mode, "WALK !");
  text_layer_set_text (l_repeat,"1/10");
  
  init_workouts () ;

  seconds_to_go = All[number].run*60;
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
  text_layer_destroy (l_mode);
  text_layer_destroy (l_repeat);

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