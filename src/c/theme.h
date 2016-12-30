#pragma once

// Themes
enum Theme {
  DARK,
  LIGHT
};

#ifndef EXTERN
#define EXTERN extern
#endif

// theme color constants
EXTERN GColor textColor, backgroundColor, foregroundColor;

// method to set application theme
void init_theme(enum Theme theme);