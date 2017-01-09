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

// Get current Theme struct
Theme* theme_get_theme();

// method to set application theme
void init_theme(enum ThemeEnum theme);

// method to initlalize custom theme with custom colors
void init_custom_theme(GColor backgroundColor, GColor foregroundColor, GColor textColor, GColor textColorInverted);