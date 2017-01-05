#include "battery_text.h"
#include "battery.h"
#include "theme.h"
#include "battery_listener.h"

// battery text layer
static TextLayer *s_battery_text_layer;

void update_battery_text(){
  // Write the current battery level into a buffer
  static char s_buffer[8];
  snprintf(s_buffer, sizeof(s_buffer), "%d%%", (int)s_battery_level);
  // update text layer
  text_layer_set_text(s_battery_text_layer, s_buffer);
}

void create_battery_text_layer(Window *window){
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  int width = bounds.size.w;
  int height = 50;
  int offsetX = 0;
  int offsetY = 0;
  
  GRect layer_bounds = GRect(offsetX, offsetY, width, height);
  // Create the TextLayer with specific bounds
  s_battery_text_layer = text_layer_create(layer_bounds);
  
  // set text properties
  text_layer_set_background_color(s_battery_text_layer, GColorClear);
  text_layer_set_text_color(s_battery_text_layer, textColor);
  text_layer_set_font(s_battery_text_layer, batteryFont);
  text_layer_set_text(s_battery_text_layer, "99%");
  text_layer_set_text_alignment(s_battery_text_layer, GTextAlignmentRight);
  
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_battery_text_layer));
}

void destroy_battery_text_layer(){
  text_layer_destroy(s_battery_text_layer);
}