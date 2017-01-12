#include <pebble.h>
#include "theme.h"

static Theme s_theme;
static enum ThemeEnum currentTheme;

static void set_fonts() {
  s_theme.DateFont = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  s_theme.TimeFont = fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD);
  //s_theme.TimeFont = fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS);
  s_theme.BatteryFont = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  s_theme.HeartrateFont = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  s_theme.WeatherFont = fonts_get_system_font(FONT_KEY_GOTHIC_18);
}

static void set_colors() {
  switch (currentTheme) {
    case LIGHT:
      s_theme.TextColor = GColorBlack;
      s_theme.TextColorInverted = GColorWhite;
      s_theme.BackgroundColor = GColorWhite;
      s_theme.ForegroundColor = GColorBlack;
    
      break;
    case DARK:
    default:
      s_theme.TextColor = GColorWhite;
      s_theme.TextColorInverted = GColorBlack;
      s_theme.BackgroundColor = GColorBlack;
      s_theme.ForegroundColor = GColorWhite;
      break;
  }
  
  // Time Layer
  s_theme.TimeTextColor = s_theme.TextColor;
  // Date Layer
  s_theme.DateTextColor = s_theme.TextColor;

  // Connection Layer
  s_theme.ConnectionIconColor = s_theme.ForegroundColor;

  // Battery Bar Layer
  s_theme.BatteryOutlineColor = s_theme.ForegroundColor;
  s_theme.BatteryFillColor = s_theme.ForegroundColor;

  // Battery Text Layer
  s_theme.BatteryTextColor = s_theme.TextColor;

  // Weather Layer
  s_theme.WeatherIconColor = s_theme.ForegroundColor;
  s_theme.WeatherTextColor = s_theme.TextColor;

  // Heartrate Layer
  #ifdef PBL_COLOR
    s_theme.HeartIconColor = GColorRed;
  #else
    s_theme.HeartIconColor = s_theme.ForegroundColor;
  #endif
}

Theme* theme_get_theme(){
  return &s_theme;
}

void init_theme(enum ThemeEnum themeEnum) {
  s_theme.CurrentThemeEnum = themeEnum;
  currentTheme = themeEnum;
  
  set_colors();
  set_fonts();
}

void init_custom_theme(Theme theme) {
  s_theme = theme;
  
  s_theme.CurrentThemeEnum = CUSTOM;
  currentTheme = CUSTOM;
  
  /*
  theme.BackgroundColor = backgroundColor;
  theme.ForegroundColor = foregroundColor;
  theme.TextColor = textColor;
  theme.TextColorInverted = textColorInverted;
  
  theme.HeartIconColor = ;
  */
  
  set_fonts();
}