#include "time.h"
#include "theme.h"

// Time TextLayer
static TextLayer *s_time_layer;

// Method to update the time textbuffer
void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, 
           sizeof(s_buffer),
           clock_is_24h_style() ? "%H:%M" : "%I:%M",
           tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}

// creates the time layer
void create_time_layer(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_unobstructed_bounds(window_layer);
  
  int width = bounds.size.w;
  int height = 50;
  int offsetX = (bounds.size.w - width) / 2;
  int offsetY = (bounds.size.h / 2) - (height / 2);
  
  GRect layer_bounds = GRect(offsetX, offsetY, width, height);
  
  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(layer_bounds);
  
  // set styling
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, textColor);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // update time value before rendering so it is shown right from the beginning
  update_time();

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

// destroys the time layer
void destroy_time_layer() {
  text_layer_destroy(s_time_layer);
}