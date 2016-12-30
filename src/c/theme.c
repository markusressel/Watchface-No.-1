#include <pebble.h>
#define EXTERN
#include "theme.h"

void init_theme(enum Theme theme) {
  switch (theme) {
    case DARK:
      textColor = GColorWhite;
      backgroundColor = GColorBlack;
      foregroundColor = textColor;
      break;
    case LIGHT:
    default:
      textColor = GColorBlack;
      backgroundColor = GColorClear;
      foregroundColor = textColor;
      break;
  }
}