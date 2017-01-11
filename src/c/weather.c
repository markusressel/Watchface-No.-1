#include <pebble-effect-layer/pebble-effect-layer.h>
#include "weather.h"
#include "clay_settings.h"
#include "theme.h"

static char s_buffer[32];

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

static int get_weather_icon_resource_id(char* conditions) {
  if (strstr(conditions, "Clear")) {
    return RESOURCE_ID_WEATHER_ICON_SUN;
  } else if (strstr(conditions, "Clouds")) {
    return RESOURCE_ID_WEATHER_ICON_CLOUDS;
  } else if (strstr(conditions, "Rain")) {
    return RESOURCE_ID_WEATHER_ICON_RAIN;
  } else if (strstr(conditions, "Thunderstorm")) {
    return RESOURCE_ID_WEATHER_ICON_THUNDERSTORM;
  } else if (strstr(conditions, "Snow")) {
    return RESOURCE_ID_WEATHER_ICON_SNOW;
  } else if (strstr(conditions, "Atmosphere")) {
    return RESOURCE_ID_WEATHER_ICON_FOG;
  } else if (strstr(conditions, "Extreme")) {
    return RESOURCE_ID_WEATHER_ICON_TORNADO;
  } else {
    return 0;
  }
}

void update_weather(){
  // Write the current temperature into a buffer
  snprintf(s_buffer, sizeof(s_buffer), "%d°C\n%s", weatherData.CurrentTemperature, weatherData.CurrentConditions);
  // update text layer
  text_layer_set_text(s_weather_layer, s_buffer);
  
  // find image matching condition
  int resourceId = get_weather_icon_resource_id(weatherData.CurrentConditions);
  
  // free any previous loaded image
  if (s_weather_icon) {
    gbitmap_destroy(s_weather_icon);
  }
  
  if (resourceId != 0) {
    // set new image
    s_weather_icon = gbitmap_create_with_resource(resourceId);
  } 
  
  // update bitmap layer
  bitmap_layer_set_bitmap(s_weather_icon_layer, s_weather_icon);
  layer_mark_dirty(bitmap_layer_get_layer(s_weather_icon_layer));
}

void create_weather_layer(Window *window){
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  GRect icon_bounds = GRect(5, bounds.size.h - 30 - 5, 30, 30);
  
  // Bitmap
  s_weather_icon_layer = bitmap_layer_create(icon_bounds);
  bitmap_layer_set_compositing_mode(s_weather_icon_layer, GCompOpSet);
  bitmap_layer_set_background_color(s_weather_icon_layer, GColorWhite); // use fixed colors, the layer will be inverted if dark theme is set
  
  s_settings = clay_get_settings();
  
  // create effect layer to modify image colors based on current theme
  s_effect_layer = effect_layer_create(icon_bounds);
  if (theme_get_theme()->CurrentThemeEnum == DARK) {
    // invert colors for dark theme
    effect_layer_add_effect(s_effect_layer, effect_invert, NULL);
  }
  
  // set bounds and offset for text layer
  int width = 60;
  int height = 36;
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