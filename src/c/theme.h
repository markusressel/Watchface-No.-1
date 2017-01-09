#pragma once
#include <pebble.h>

typedef struct Theme {
  GColor BackgroundColor;
  GColor ForegroundColor;
  GColor TextColor;
  GColor TextColorInverted;
  
  GFont TimeFont;
  GFont DateFont;
  GFont BatteryFont;
  GFont HeartrateFont;
  GFont WeatherFont;
} __attribute__((__packed__)) Theme;

// Themes
enum ThemeEnum {
  DARK,
  LIGHT
};

// Get current Theme struct
Theme* theme_get_theme();

// method to set application theme
void init_theme(enum ThemeEnum theme);