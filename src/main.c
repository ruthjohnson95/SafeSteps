#include <pebble.h>

Window* g_window;
TextLayer *g_text_layer;
TextLayer *title_layer, *location_layer, *temperature_layer, *time_layer;

static TextLayer* init_text_layer(GRect location, GColor colour, GColor background, const char *res_id, GTextAlignment alignment)
{
  TextLayer *layer = text_layer_create(location);
  text_layer_set_text_color(layer, colour);
  text_layer_set_background_color(layer, background);
  text_layer_set_font(layer, fonts_get_system_font(res_id));
  text_layer_set_text_alignment(layer, alignment);
 
  return layer;
}

void window_load(Window *window)
{
  title_layer = init_text_layer(GRect(5, 0, 144, 30), GColorBlack, GColorClear, "RESOURCE_ID_GOTHIC_18", GTextAlignmentLeft);
  text_layer_set_text(title_layer, "Openweathermap.org");
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(title_layer));
  
  location_layer = init_text_layer(GRect(5, 30, 144, 30), GColorBlack, GColorClear, "RESOURCE_ID_GOTHIC_18", GTextAlignmentLeft);
  text_layer_set_text(location_layer, "Location: N/A");
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(location_layer));
 
  temperature_layer = init_text_layer(GRect(5, 60, 144, 30), GColorBlack, GColorClear, "RESOURCE_ID_GOTHIC_18", GTextAlignmentLeft);
  text_layer_set_text(temperature_layer, "Temperature: N/A");
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(temperature_layer));
 
  time_layer = init_text_layer(GRect(5, 90, 144, 30), GColorBlack, GColorClear, "RESOURCE_ID_GOTHIC_18", GTextAlignmentLeft);
  text_layer_set_text(time_layer, "Last updated: N/A");
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
}

/*
void window_load(Window *window){
  // we will add the creation of the window's element here
  g_text_layer = text_layer_create(GRect(0, 0, 144, 168));
  text_layer_set_background_color(g_text_layer, GColorClear);
  text_layer_set_text_color(g_text_layer, GColorBlack);
 
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(g_text_layer));
  text_layer_set_text(g_text_layer, "Welcome to SafeSteps");
}
*/

void window_unload(Window *window)
{
  text_layer_destroy(title_layer);
  text_layer_destroy(location_layer);
  text_layer_destroy(temperature_layer);
  text_layer_destroy(time_layer);
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