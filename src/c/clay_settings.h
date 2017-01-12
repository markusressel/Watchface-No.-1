#pragma once

// Persistent storage key
#define SETTINGS_KEY 1

// Define our settings struct
typedef struct ClaySettings {
  GColor BackgroundColor;
  GColor ForegroundColor;
  GColor TextColor;
  GColor TextColorInverted;
  
  // Time Layer
  GColor TimeTextColor;
  
  // Date Layer
  GColor DateTextColor;
  
  // Connection Layer
  GColor ConnectionIconColor;
  
  // Battery Bar Layer
  GColor BatteryFrameColor;
  GColor BatteryFillColor;
  
  // Battery Text Layer
  GColor BatteryTextColor;
  
  // Weather Layer
  GColor WeatherIconColor;
  GColor WeatherTextColor;
  
  // Heartrate Layer
  GColor HeartIconColor;
  
  char ThemeValue[10];
  
  bool ShowSeconds;
  bool ShowAnimations;
} __attribute__((__packed__)) ClaySettings;


// get current settings struct
ClaySettings* clay_get_settings();

// save current current settings struct to persistent storage
void clay_save_settings();

// load settings from persistent storage
void clay_load_settings();