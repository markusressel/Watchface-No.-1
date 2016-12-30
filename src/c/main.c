#include <pebble.h>
#include "time.h"
#include "date.h"
#include "battery_bar.h"
#include "battery_text.h"
#include "tickListener.h"
#include "batteryListener.h"
#include "theme.h"

// Main Window
static Window *s_main_window;

// loads componentents into the main window
static void main_window_load(Window *window) {
  // Create layers
  create_time_layer(window);
  create_date_layer(window);
  create_battery_bar_layer(window);
  create_battery_text_layer(window);
  
  // Register for tick events (time)
  register_tick_listener();
  // Register for battery level updates
  register_battery_listener();
  // Ensure battery level is displayed from the start
  force_battery_update();
}

// destroys all components of the main window
static void main_window_unload(Window *window) {
  // unregister listeners
  unregister_tick_listener();
  unregister_battery_listener();
  
  // destroy layers
  destroy_battery_text_layer();
  destroy_battery_bar_layer();
  destroy_date_layer();
  destroy_time_layer();
}

// initializes the watchface
static void init() {
  // set theme
  enum Theme theme = DARK;
  init_theme(theme);
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  // Apply theme
  window_set_background_color(s_main_window, backgroundColor);
  
  // Set handler to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}

// deinitializes the watchface
static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

// watchface lifecycle
int main(void) {
  init();
  app_event_loop();
  deinit();
}