#include <pebble.h>
#include "theme.h"

static Theme theme;
static enum ThemeEnum currentTheme;

static void set_fonts() {
  theme.DateFont = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  theme.TimeFont = fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD);
  //timeFont = fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS);
  theme.BatteryFont = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  theme.HeartrateFont = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  theme.WeatherFont = fonts_get_system_font(FONT_KEY_GOTHIC_18);
}

static void set_colors() {
  switch (currentTheme) {
    case DARK:
      theme.TextColor = GColorWhite;
      theme.TextColorInverted = GColorBlack;
      theme.BackgroundColor = GColorBlack;
      theme.ForegroundColor = GColorWhite;
      break;
    case LIGHT:
    default:
      theme.TextColor = GColorBlack;
      theme.TextColorInverted = GColorWhite;
      theme.BackgroundColor = GColorWhite;
      theme.ForegroundColor = GColorBlack;
      break;
  } 
}

Theme* theme_get_theme(){
  return &theme;
}

void init_theme(enum ThemeEnum theme) {
 //  s_settings = clay_get_settings();
  currentTheme = theme;
  
  set_colors();
  set_fonts();
}