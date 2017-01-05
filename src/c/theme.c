#include <pebble.h>
#define EXTERN
#include "theme.h"

static void set_fonts() {
  dateFont = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  timeFont = fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS);
  batteryFont = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  heartrateFont = fonts_get_system_font(FONT_KEY_GOTHIC_18);
}

static void set_colors() {
  switch (appTheme) {
    case DARK:
      textColor = GColorWhite;
      backgroundColor = GColorBlack;
      foregroundColor = textColor;
      break;
    case LIGHT:
    default:
      textColor = GColorBlack;
      backgroundColor = GColorWhite;
      foregroundColor = textColor;
      break;
  } 
}

void init_theme(enum Theme theme) {
  appTheme = theme;
  
  set_colors();
  set_fonts();
}