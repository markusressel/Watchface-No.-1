#pragma once

// Persistent storage key
#define SETTINGS_KEY 1

// Define our settings struct
typedef struct ClaySettings {
  GColor BackgroundColor;
  GColor ForegroundColor;
  GColor TextColor;
  bool ShowSeconds;
  bool ShowAnimations;
} __attribute__((__packed__)) ClaySettings;


// get current settings struct
ClaySettings* clay_get_settings();

// save current current settings struct to persistent storage
void clay_save_settings();

// load settings from persistent storage
void clay_load_settings();