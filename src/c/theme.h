#pragma once
#include <pebble.h>

// Themes
enum ThemeEnum {
  DARK,
  LIGHT,
  CUSTOM
};

typedef struct Theme {
  enum ThemeEnum CurrentThemeEnum;
  
  // General Colors
  GColor BackgroundColor;
  GColor ForegroundColor;
  GColor TextColor;
  
  // Time Layer
  GColor TimeTextColor;
  // Date Layer
  GColor DateTextColor;
  
  // Connection Layer
  GColor ConnectionIconColor;
    
  // Battery Bar Layer
  GColor BatteryOutlineColor;
  GColor BatteryFillColor;
  
  // Battery Text Layer
  GColor BatteryTextColor;
  
  // Weather Layer
  GColor WeatherIconColor;
  GColor WeatherTextColor;
  
  // Heartrate layer
  GColor HeartIconColor;
  GColor HeartrateTextColor;
  
  // Fonts
  GFont TimeFont;
  GFont DateFont;
  GFont BatteryFont;
  GFont HeartrateFont;
  GFont WeatherFont;
} __attribute__((__packed__)) Theme;

// Get current Theme struct
Theme* theme_get_theme();

// method to set application theme
void init_theme(enum ThemeEnum theme);

// method to initlalize custom theme with custom colors
void init_custom_theme(Theme theme);