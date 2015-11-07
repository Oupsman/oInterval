#include <pebble.h>
#include "exercice_window.h"

Window* window;
 
MenuLayer *menu_layer;

void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
    //Which row is it?
    switch(cell_index->row)
    {
    case 0:
        menu_cell_basic_draw(ctx, cell_layer, "Very beginner", "2 minutes run, 1 minute walk, 10 times", NULL);
        break;
    case 1:
        menu_cell_basic_draw(ctx, cell_layer, "Beginner 1", "3 minutes run, 1 minute walk, 10 times", NULL);
        break;
    case 2:
        menu_cell_basic_draw(ctx, cell_layer, "Beginner 2", "5 minutes run, 2 minutes walk, 7 times", NULL);
        break;
    case 3:
        menu_cell_basic_draw(ctx, cell_layer, "Beginner 3", "10 minutes run, 3 minutes walk, 4 times", NULL);
        break;
    case 4:
        menu_cell_basic_draw(ctx, cell_layer, "Intermediate", "15 minutes run, 5 minutes walk, 3 times", NULL);
        break;
    case 5:
        menu_cell_basic_draw (ctx, cell_layer, "Custom", "Customize your workout", NULL);
      break;
    default:
        APP_LOG (APP_LOG_LEVEL_INFO, "ARE YOU KIDDING ME ?!");
        break;
    }
}

uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context) {
 
  return 5;

}
 
void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
 
  //Get which row
	uint8_t which = cell_index->row;
  if (which < 5) {
    start_workout (which);
  } /*else {
    start_custom ();
  }*/
  
}

void window_load(Window *window) {
  
  Layer *window_layer = window_get_root_layer (window);
  GRect bounds = layer_get_bounds(window_layer);
  
  //Create it - 16 is approx height of the top bar
	menu_layer = menu_layer_create(bounds);
	
	//Let it receive clicks
	menu_layer_set_click_config_onto_window(menu_layer, window);
	
	//Give it its callbacks
	MenuLayerCallbacks callbacks = {
		.draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
		.get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback,
		.select_click = (MenuLayerSelectCallback) select_click_callback
	};
	menu_layer_set_callbacks(menu_layer, NULL, callbacks);
	
	//Add to Window
	layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));

}
 
void window_unload(Window *window) {
 
  menu_layer_destroy (menu_layer);

}

void init() {

  window = window_create();
  WindowHandlers handlers = {
      .load = window_load,
      .unload = window_unload
  };
  window_set_window_handlers(window, (WindowHandlers) handlers);
  window_stack_push(window, true);

}
 
void deinit() {

  window_destroy(window);
  
}
 
int main(void) {
    init();
    app_event_loop();
    deinit();
}