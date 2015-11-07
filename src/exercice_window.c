#include <pebble.h>
#include "exercice_window.h"


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

}

static void workout_up_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG (APP_LOG_LEVEL_INFO, "UP button pressed");
}

static void workout_select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (is_counting) {
    tick_timer_service_unsubscribe();
    is_counting = false;
    gbitmap_destroy (s_icon);
    s_icon = gbitmap_create_with_resource (RESOURCE_ID_ICON_STOP);
    bitmap_layer_set_bitmap (s_icon_layer, s_icon);
    
  } else {
    tick_timer_service_subscribe (SECOND_UNIT, &do_workout);
    is_counting = true;
    gbitmap_destroy (s_icon);
    if (is_running) {
      s_icon = gbitmap_create_with_resource (RESOURCE_ID_ICON_RUN);
    } else {
      s_icon = gbitmap_create_with_resource (RESOURCE_ID_ICON_WALK); 
    }
  }
}

static void workout_down_click_handler(ClickRecognizerRef recognizer, void *context) {
 APP_LOG (APP_LOG_LEVEL_INFO,"Down button pressed !") ;
}

static void workout_click_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, workout_up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, workout_select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, workout_down_click_handler);
  APP_LOG (APP_LOG_LEVEL_INFO,"Done registering handlers");
}

void update_screen (Layer *me, GContext *ctx) {
  
  uint8_t i;
  
  uint16_t total; 
  uint32_t start_angle_run, stop_angle_run, start_angle_walk; 
  
  GRect bounds = layer_get_bounds (me);
  
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
    graphics_draw_arc (ctx, 
                       GRect (5,5,bounds.size.w-10,bounds.size.h-10), 
                       GOvalScaleModeFitCircle, 
                       DEG_TO_TRIGANGLE(start_angle_run), 
                       DEG_TO_TRIGANGLE(stop_angle_run)
                      );
    
  }
  
  graphics_context_set_stroke_color (ctx,GColorGreen);
  
  graphics_draw_arc (ctx, 
                     GRect (5,5,bounds.size.w-10,bounds.size.h-10), 
                     GOvalScaleModeFitCircle, 
                     DEG_TO_TRIGANGLE(start_angle_walk), 
                     DEG_TO_TRIGANGLE(360));
  
  graphics_context_set_stroke_color (ctx,GColorBlack);
  graphics_context_set_stroke_width (ctx, 2); 
  
  
  for (i=1; i<=All[number].repeat; i++) {
    
    uint16_t start_angle, stop_angle;
  
    start_angle = 360*i/All[number].repeat - 360/All[number].repeat;
    stop_angle = 360*i/All[number].repeat-5;
    if (i < current_cycle) {
      graphics_context_set_stroke_color (ctx, GColorBlack);
    } else if (i == current_cycle) {
      graphics_context_set_stroke_color (ctx, GColorRed);
    } else {
      graphics_context_set_stroke_color (ctx,GColorLightGray);
    }
    graphics_draw_arc (ctx,
                      GRect (10,10,bounds.size.w-20,bounds.size.h-20), 
                       GOvalScaleModeFitCircle, 
                       DEG_TO_TRIGANGLE (start_angle), 
                       DEG_TO_TRIGANGLE(stop_angle)
                      );   
    
  }

}

void start_workout (uint8_t which) {
  
  number = which;
  
  workout_window_init ();
 
}

void do_workout (struct tm *tick_time, TimeUnits units_changed) {
  
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
        gbitmap_destroy (s_icon);
        s_icon = gbitmap_create_with_resource (RESOURCE_ID_ICON_RUN);
        bitmap_layer_set_bitmap (s_icon_layer, s_icon);
        is_running = 1;
        seconds_to_go = All[number].run;
        current_cycle++;
        break;
      case 1:
        gbitmap_destroy (s_icon);
        s_icon = gbitmap_create_with_resource (RESOURCE_ID_ICON_WALK);
        bitmap_layer_set_bitmap (s_icon_layer, s_icon);
        is_running = 0;
        seconds_to_go = All[number].walk;
        break;
      
    }
  }
  layer_mark_dirty (s_funky_layer);
}

void workout_window_load (Window *window) {
  
  Layer *window_layer = window_get_root_layer (window);
  GRect bounds = layer_get_bounds(window_layer);
  
  l_timetogo = text_layer_create (GRect((bounds.size.w-80)/2, bounds.size.h/2-50, 80, 30));
  s_funky_layer = layer_create (bounds);
  l_repeat = text_layer_create (GRect((bounds.size.w-80)/2,bounds.size.h/2-20,80,30));

  s_icon_layer = bitmap_layer_create (GRect((bounds.size.w-40)/2,bounds.size.h/2+10,40,40));
  
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
  layer_add_child (window_get_root_layer(s_workout_window), bitmap_layer_get_layer (s_icon_layer));
  
  init_workouts () ;

  seconds_to_go = All[number].run;
  is_running = 1;
  current_cycle = 1;
  
  time_run = All[number].run;
  time_walk = All[number].walk;
  
  minutes = seconds_to_go / 60;
  seconds = seconds_to_go % 60;
  
  is_counting = 1;
  
  s_icon = gbitmap_create_with_resource (RESOURCE_ID_ICON_RUN);
  
  bitmap_layer_set_bitmap(s_icon_layer, s_icon);

  tick_timer_service_subscribe (SECOND_UNIT, &do_workout);
  
}

void workout_window_unload () {
  
  tick_timer_service_unsubscribe ();  
  
  gbitmap_destroy (s_icon); 
  
  text_layer_destroy (l_timetogo);
  text_layer_destroy (l_repeat);
  bitmap_layer_destroy (s_icon_layer);
  
  layer_destroy (s_funky_layer);
  
  window_destroy(s_workout_window);
  
}

void workout_window_init () {
    
  s_workout_window = window_create();
  WindowHandlers handlers = {
      .load = workout_window_load,
      .unload = workout_window_unload
  };
  
  window_set_window_handlers(s_workout_window, (WindowHandlers) handlers);
  window_set_click_config_provider(s_workout_window, workout_click_provider);
  
  window_stack_push(s_workout_window, true);  
  
}