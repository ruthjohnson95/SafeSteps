#include <pebble.h>

enum WeatherKey {
  WEATHER_ICON_KEY = 0x0,         // TUPLE_INT
  WEATHER_TEMPERATURE_KEY = 0x1,  // TUPLE_CSTRING
  WEATHER_CITY_KEY = 0x2,         // TUPLE_CSTRING
};

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void request_weather(void) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (!iter) {
    // Error creating outbound message
    return;
  }

  int value = 1;
  dict_write_int(iter, 1, &value, sizeof(int), true);
  dict_write_end(iter);

  app_message_outbox_send();
}

Window* g_window;
static TextLayer *s_time_layer;
static GFont s_time_font;
TextLayer *title_layer, *location_layer, *temperature_layer, *time_layer;
char location_buffer[64], temperature_buffer[32], time_buffer[32];

enum {
  KEY_LOCATION = 0,
  KEY_TEMPERATURE = 1,
};

void process_tuple(Tuple *t)
{
  //Get key
  int key = t->key;
 
  //Get integer value, if present
  int value = t->value->int32;
 
  //Get string value, if present
  char string_value[32];
  strcpy(string_value, t->value->cstring);
 
  //Decide what to do
  switch(key) {
    case KEY_LOCATION:
      //Location received
      snprintf(location_buffer, sizeof("Location: couldbereallylongname"), "Location: %s", string_value);
      text_layer_set_text(location_layer, (char*) &location_buffer);
      break;
    case KEY_TEMPERATURE:
      //Temperature received
      snprintf(temperature_buffer, sizeof("Temperature: XX \u00B0C"), "Temperature: %d \u00B0C", value);
      text_layer_set_text(temperature_layer, (char*) &temperature_buffer);
      break;
  }
  
  //Set time this update came in
  time_t temp = time(NULL);
  struct tm *tm = localtime(&temp);
  strftime(time_buffer, sizeof("Last updated: XX:XX"), "Last updated: %H:%M", tm);
  text_layer_set_text(time_layer, (char*) &time_buffer);
}
  
static void in_received_handler(DictionaryIterator *iter, void *context)
{
  (void) context;
     
    //Get data
    Tuple *t = dict_read_first(iter);
    while(t != NULL)
    {
        process_tuple(t);
         
        //Get next
        t = dict_read_next(iter);
    }
}


static TextLayer* init_text_layer(GRect location, GColor colour, GColor background, const char *res_id, GTextAlignment alignment)
{
  TextLayer *layer = text_layer_create(location);
  text_layer_set_text_color(layer, colour);
  text_layer_set_background_color(layer, background);
  text_layer_set_font(layer, fonts_get_system_font(res_id));
  text_layer_set_text_alignment(layer, alignment);
 
  return layer;
}

/* update time */
static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

/* tick timer handler*/
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

/* Button clicks initialization */
void up_click_handler(ClickRecognizerRef recognizer, void *context){
  text_layer_set_text(s_time_layer, "SENT");
}

void down_click_handler(ClickRecognizerRef recognizer, void *context){
  text_layer_set_text(s_time_layer, "SENT");
}

void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(s_time_layer, "SENT");
}

/* Register clicks with the system */
void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

/* Window Load/Unload functions */
static void window_load(Window *window) {  
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);

  // Improve the layout to be more like a watchface
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_KEY_ROBOTO_MEDIUM_40));
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  request_weather();
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

static void window_unload(Window *window) {
    // Destroy TextLayer
    text_layer_destroy(s_time_layer);
    fonts_unload_custom_font(s_time_font);

}

void init(){
  // create app elements here
  g_window = window_create();
  window_set_window_handlers(g_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  //Setting up AppMessage
  //Register AppMessage events
app_message_register_inbox_received(in_received_handler);
app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());    
  //Largest possible input and output buffer sizes
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // enable click function to set up click behavior 
  window_set_click_config_provider(g_window, click_config_provider);
  
  // make the app actually appear when it is chosen from the Pebble menu
  window_stack_push(g_window, true);
  
  // Make sure the time is displayed from the start
  update_time();
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