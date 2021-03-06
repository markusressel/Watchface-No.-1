#include <pebble.h>
#include "date.h"
#include "clay_settings.h"
#include "theme.h"

static ClaySettings *s_settings;

// Date TextLayer
static TextLayer *s_date_layer;

void update_date() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[16];
  strftime(s_buffer, 
           sizeof(s_buffer),
          "%a, %d.%m",
           tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_date_layer, s_buffer);
}

// creates the date layer
void create_date_layer(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  s_settings = clay_get_settings();
  
  int width = bounds.size.w;
  int height = 35;
  int offsetX = (bounds.size.w - width) / 2;
  int offsetY = (bounds.size.h / 2) - 55;
  
  GRect layer_bounds = GRect(offsetX, offsetY, width, height);
  
  // Create the TextLayer with specific bounds
  s_date_layer = text_layer_create(layer_bounds);
  
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_date_layer, GColorClear);
  //text_layer_set_background_color(s_date_layer, theme_get_theme()->BackgroundColor);
  text_layer_set_text_color(s_date_layer, theme_get_theme()->DateTextColor);
  text_layer_set_font(s_date_layer, theme_get_theme()->DateFont);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  
  update_date();

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
}

// destroys the date layer
void destroy_date_layer() {
  text_layer_destroy(s_date_layer);
}