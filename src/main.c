#include <pebble.h>

Window* g_window;
TextLayer *g_text_layer;

void window_load(Window *window){
  // we will add the creation of the window's element here
  g_text_layer = text_layer_create(GRect(0, 0, 144, 168));
  text_layer_set_background_color(g_text_layer, GColorClear);
  text_layer_set_text_color(g_text_layer, GColorBlack);
 
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(g_text_layer));
  text_layer_set_text(g_text_layer, "Welcome to SafeSteps");
}

void window_unload(Window *window){
  // we will destroy the window element here
  text_layer_destroy(g_text_layer);
}
  
void init(){
  // create app elements here
  g_window = window_create();
  window_set_window_handlers(g_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  // make the app actually appear when it is chosen from the Pebble menu
  window_stack_push(g_window, true);
}

void deinit(){
  // destroy app elements here
  window_destroy(g_window);
}

int main (void){
  init();
  app_event_loop();
  deinit();
}