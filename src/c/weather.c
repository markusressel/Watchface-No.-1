#include <pebble-effect-layer/pebble-effect-layer.h>
#include "weather.h"
#include "clay_settings.h"
#include "theme.h"

// static char s_temperature_buffer[8];
// static char s_conditions_buffer[32];

WeatherData weatherData;

static ClaySettings *s_settings;

// Weather TextLayer
static TextLayer *s_weather_layer;

// Weather icon Bitmap
static GBitmap *s_weather_icon;

// Icon layer
static BitmapLayer *s_weather_icon_layer;

// Effect layer, covering the weather icon to change image color
static EffectLayer *s_effect_layer;

WeatherData* weather_get_data() {
  return &weatherData;
}

void update_weather(){
  // snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
  // text_layer_set_text(s_weather_layer, weather_layer_buffer);
  
  // Write the current temperature into a buffer
  static char s_buffer[8];
  snprintf(s_buffer, sizeof(s_buffer), "%d°C", weatherData.CurrentTemperature);
  // update text layer
  text_layer_set_text(s_weather_layer, s_buffer);
}

void create_weather_layer(Window *window){
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  GRect icon_bounds = GRect(5, bounds.size.h - 30, 0, 0);
  
  // Bitmap
  s_weather_icon = gbitmap_create_with_resource(RESOURCE_ID_WEATHER_ICON_SUN);
  s_weather_icon_layer = bitmap_layer_create(icon_bounds);
  bitmap_layer_set_compositing_mode(s_weather_icon_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_weather_icon_layer, s_weather_icon);
  bitmap_layer_set_background_color(s_weather_icon_layer,GColorWhite); // use fixed colors, the layer will be inverted if dark theme is set
  
  s_settings = clay_get_settings();
  
  // create effect layer to modify image colors based on current theme
  s_effect_layer = effect_layer_create(icon_bounds);
  if (theme_get_theme()->CurrentThemeEnum == DARK) {
    // invert colors for dark theme
    effect_layer_add_effect(s_effect_layer, effect_invert, NULL);
  }
  
  // set bounds and offset for text layer
  int width = 60;
  int height = 25;
  int offsetX = icon_bounds.size.w + 5 + 5;
  int offsetY = bounds.size.h - height - 5;
  
  GRect layer_bounds = GRect(offsetX, offsetY, width, height);
  
  // Create temperature Layer
  s_weather_layer = text_layer_create(layer_bounds);

  // Style the text
  text_layer_set_background_color(s_weather_layer, theme_get_theme()->BackgroundColor);
  text_layer_set_text_color(s_weather_layer, theme_get_theme()->TextColor);
  text_layer_set_font(s_weather_layer, theme_get_theme()->WeatherFont);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentLeft);
  text_layer_set_text(s_weather_layer, "---");
  
  layer_add_child(window_layer, bitmap_layer_get_layer(s_weather_icon_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_weather_layer));
  layer_add_child(window_layer, effect_layer_get_layer(s_effect_layer));
}

void destroy_weather_layer(){
  // destroy current weather icon
  gbitmap_destroy(s_weather_icon);
  
  // destroy weather icon layer
  bitmap_layer_destroy(s_weather_icon_layer);
  effect_layer_destroy(s_effect_layer);
  
  // destroy weather temperature layer
  text_layer_destroy(s_weather_layer);
}