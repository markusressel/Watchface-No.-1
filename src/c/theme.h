#pragma once

// Themes
enum Theme {
  DARK,
  LIGHT
};

#ifndef EXTERN
#define EXTERN extern
#endif

EXTERN enum Theme appTheme;

// theme color constants
EXTERN GColor textColor, textColorInverted, backgroundColor, foregroundColor;

// fonts
EXTERN GFont dateFont, timeFont, batteryFont, heartrateFont, weatherFont, heartrateFont;

// method to set application theme
void init_theme(enum Theme theme);