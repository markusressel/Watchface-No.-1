#include <pebble.h>
#include "time.h"
#include "date.h"
#include "battery_bar.h"
#include "battery_text.h"
#include "tick_listener.h"
#include "battery_listener.h"
#include "phone_connection_indicator.h"
#include "system_event_listener.h"
#include "weather.h"
#include "health_listener.h"
#include "heartrate.h"
#include "app_messaging.h"
#include "clay_settings.h"
#include "theme.h"

// Main Window
static Window *s_main_window;

static ClaySettings *s_settings;

// loads componentents into the main window
static void main_window_load(Window *window) {
  // Create layers
  APP_LOG(APP_LOG_LEVEL_DEBUG, "initializing time layer");
  create_time_layer(window);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "initializing date layer");
  create_date_layer(window);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "initializing battery bar layer");
  create_battery_bar_layer(window);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "initializing battery text layer");
  create_battery_text_layer(window);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "initializing phone connection layer");
  create_phone_connection_indicator_layer(window);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "initializing weather layer");
  create_weather_layer(window);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "initializing heartrate layer");
  create_heartrate_layer(window);
  
  // Register for tick events (time)
  register_tick_listener();
  // Register for battery level updates
  register_battery_listener();
  // Ensure battery level is displayed from the start
  force_battery_update();
  // Register system event handler
  register_system_event_listener();
  // Register health event listener
  register_health_event_listener();
  
  initialize_app_messaging();
}

// destroys all components of the main window
static void main_window_unload(Window *window) {
  // unregister listeners
  unregister_tick_listener();
  unregister_battery_listener();
  unregister_system_event_listener();
  unregister_health_event_listener();
  
  // destroy layers
  destroy_heartrate_layer();
  destroy_weather_layer();
  destroy_phone_connection_indicator_layer();
  destroy_battery_text_layer();
  destroy_battery_bar_layer();
  destroy_date_layer();
  destroy_time_layer();
}

// initializes the watchface
static void init() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "initializing settings");
  // load clay configuration
  clay_load_settings();
  s_settings = clay_get_settings();
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "mapping theme value to enum");
  // map clay configuration value to ThemeEnum
  
  if (!s_settings) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Settings are NULL!");
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Settings are valid :)");
    if (!s_settings->ThemeValue) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "ThemeValue is NULL!");
    } else {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "ThemeValue is valid :)");
    }
  }
  
  if (!s_settings->ThemeValue) {
    strcpy(s_settings->ThemeValue, "DARK");
  }
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "ThemeValue: %s", s_settings->ThemeValue);
  enum ThemeEnum theme;
  if (strcmp(s_settings->ThemeValue, "LIGHT") == 0) {
    theme = LIGHT;
  } else if (strcmp(s_settings->ThemeValue, "CUSTOM") == 0) {
    theme = CUSTOM;
  } else {
    theme = DARK;
  }
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "initializing theme");
  // initialize theme based on ThemeEnum
  if (theme == CUSTOM) {
    Theme custom_theme;
    custom_theme.BackgroundColor = s_settings->BackgroundColor;
    custom_theme.ForegroundColor = s_settings->ForegroundColor;
    custom_theme.TextColor = s_settings->TextColor;
    custom_theme.TextColorInverted = s_settings->TextColorInverted;
    
    custom_theme.HeartIconColor = s_settings->HeartIconColor;
    
    init_custom_theme(custom_theme);
  } else {
    init_theme(theme);
  }
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "applying window background");
  // Apply theme
  window_set_background_color(s_main_window, theme_get_theme()->BackgroundColor);
  
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