#include <pebble.h>
#include "heartrate.h"
#include "health_listener.h"
#include "theme.h"

// Heartrate TextLayer
static TextLayer *s_heartrate_layer;

// Method to update the heartrate textbuffer
void update_heartrate() {
  // Write the current heartrate into a buffer
  static char s_buffer[8];
  snprintf(s_buffer, sizeof(s_buffer), "%d BPM", (int)s_heartrate_bpm);
  
  // Display this heartrate on the TextLayer
  text_layer_set_text(s_heartrate_layer, s_buffer);
}

// creates the layer
void create_heartrate_layer(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  int width = 50;
  int height = 30;
  int offsetX = bounds.size.w - width;
  int offsetY = bounds.size.h - height;
  
  GRect layer_bounds = GRect(offsetX, offsetY, width, height);
  
  // Create the TextLayer with specific bounds
  s_heartrate_layer = text_layer_create(layer_bounds);
  
  // set styling
  text_layer_set_background_color(s_heartrate_layer, backgroundColor);
  text_layer_set_text_color(s_heartrate_layer, textColor);
  text_layer_set_font(s_heartrate_layer, heartrateFont);
  text_layer_set_text_alignment(s_heartrate_layer, GTextAlignmentCenter);
  
  // update value before rendering so it is shown right from the beginning
  update_heartrate();

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_heartrate_layer));
}

// destroys the time layer
void destroy_heartrate_layer() {
  text_layer_destroy(s_heartrate_layer);
}