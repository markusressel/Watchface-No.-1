#include <pebble.h>
#include "clay_settings.h"

// An instance of the struct
static ClaySettings settings;

// Initialize the default settings
static void clay_default_settings() {
  settings.ShowSeconds = false;
  settings.ShowAnimations = true;
  
  // set theme
  // enum ThemeValues theme = DARK;
  //settings.ThemeValue = theme;
  settings.ThemeValue = "DARK";
}

ClaySettings* clay_get_settings() {
  return &settings;
}

// Read settings from persistent storage
void clay_load_settings() {
  // Load the default settings
  clay_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

void clay_save_settings() {
  // save ClaySettings struct to persistent storage
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}