#include <pebble.h>
#include "custom.h"
#include "storage.h"

static void custom_up_click_handler(ClickRecognizerRef recognizer, void *context) {
  
  uint8_t increment;
  
  switch (field) {
    case 0:
    case 2:
      increment = 1;
    break;
    case 1:
    case 3:
      increment = 10;
    break;
    default:
    break;
    
  }
  
  
}

static void custom_select_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG (APP_LOG_LEVEL_INFO,"Select button pressed !"); 
  field++;
  if (field > max_fields) {
    field = 1;
  }
  custom_update_view();
  
}

static void custom_down_click_handler(ClickRecognizerRef recognizer, void *context) {
  uint8_t increment;
  
  switch (field) {
    case 0:
    case 2:
      increment = 1;
    break;
    case 1:
    case 3:
      increment = 10;
    break;
    default:
    break;
    
  }
}

static void custom_click_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, custom_up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, custom_select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, custom_down_click_handler);
  APP_LOG (APP_LOG_LEVEL_INFO,"Done registering handlers");
}


void custom_update_view () {
  
  switch (field) {
    case 1:
    break;
    case 2:
    break;
    case 3:
    break;
    case 4:
    break;
    case 5:
    break;
    
    default:
      APP_LOG (APP_LOG_LEVEL_INFO, "Field %u unknow", field);
    break;
  }
}

void custom_window_load (Window *window) {
  
  Layer *window_layer = window_get_root_layer (window);
  GRect bounds = layer_get_bounds(window_layer);

  s_label_run     = text_layer_create(GRect(0,0,bounds.size.w/2, 25));
  s_label_walk    = text_layer_create(GRect(0,0,bounds.size.w/2, 25));
  s_label_repeat  = text_layer_create(GRect(0,0,bounds.size.w/2, 25));
  
  text_layer_set_text(s_label_run, "Running time");
  text_layer_set_text(s_label_walk, "Walking time");
  text_layer_set_text(s_label_repeat, "# of iterations");
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_label_run));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_label_walk));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_label_repeat));               
  
  field = 1;
  
  custom_update_view ();
  
}

void custom_window_unload () {
  
  window_destroy(s_custom_window);
  
}

void custom_window_init () {
    
  s_custom_window = window_create();
  WindowHandlers handlers = {
      .load = custom_window_load,
      .unload = custom_window_unload
  };
  
  window_set_window_handlers(s_custom_window, (WindowHandlers) handlers);
  window_set_click_config_provider(s_custom_window, custom_click_provider);
  
  window_stack_push(s_custom_window, true);  
  
}

void start_custom (void) {
  
  
}
