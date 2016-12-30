#include "weather.h"
#include "theme.h"

// Weather TextLayer
static TextLayer *s_weather_layer;

// Weather Font
static GFont s_weather_font;

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {

}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void update_weather(){
  // Write the current temperature into a buffer
  static char s_buffer[8];
  snprintf(s_buffer, sizeof(s_buffer), "33°C");
  // update text layer
  text_layer_set_text(s_weather_layer, s_buffer);
}

static void initialize_app_message() {
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  const int inbox_size = 128;
  const int outbox_size = 128;
  app_message_open(inbox_size, outbox_size);
}

void create_weather_layer(Window *window){
  initialize_app_message();
  
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_unobstructed_bounds(window_layer);
  
  int width = bounds.size.w * 0.3;
  int height = 25;
  int offsetX = 0;
  int offsetY = 0;
  
  GRect layer_bounds = GRect(offsetX, offsetY, width, height);
  
  // Create temperature Layer
  s_weather_layer = text_layer_create(layer_bounds);

  // Style the text
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, textColor);
  text_layer_set_font(s_weather_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentLeft);
  text_layer_set_text(s_weather_layer, "Loading...");
  
  layer_add_child(window_layer, text_layer_get_layer(s_weather_layer));
}

void destroy_weather_layer(){
  text_layer_destroy(s_weather_layer);
}