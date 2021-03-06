#include "weather.h"
#include "clay_settings.h"
#include "theme.h"
#include "gbitmap_color_palette_manipulator.h"

static char s_buffer[32];

static WeatherData weatherData;

static ClaySettings *s_settings;

// Timer to update weather after given amount of time
static int s_weather_update_interval = 1800000;
//static int s_weather_update_interval = 30000;
static AppTimer *s_update_timer;

// Weather TextLayer
static TextLayer *s_weather_layer;

// Weather icon Bitmap
static GBitmap *s_weather_icon;

// Icon layer
static BitmapLayer *s_weather_icon_layer;

WeatherData* weather_get_data() {
  return &weatherData;
}

static void restore_saved_weather_data() {
  // Read settings from persistent storage, if they exist
  persist_read_data(WEATHER_DATA_KEY, &weatherData, sizeof(weatherData));
}

static void save_current_weather_data() {
  // save WeatherData struct to persistent storage
  persist_write_data(WEATHER_DATA_KEY, &weatherData, sizeof(weatherData));
}

static int get_weather_icon_resource_id(char* conditions) {
  if (strstr(conditions, "Clear")) {
    return RESOURCE_ID_WEATHER_ICON_SUN;
  } else if (strstr(conditions, "Clouds")) {
    return RESOURCE_ID_WEATHER_ICON_CLOUDS;
  } else if (strstr(conditions, "Drizzle")) {
    return RESOURCE_ID_WEATHER_ICON_DRIZZLE;
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
  } else if (strstr(conditions, "Additional")) {
    return RESOURCE_ID_WEATHER_ICON_WIND;
  } else {
    return 0;
  }
}

static void request_weather_update() {
  // Declare the dictionary's iterator
  DictionaryIterator *out_iter;

  // Prepare the outbox buffer for this message
  AppMessageResult result = app_message_outbox_begin(&out_iter);
  if(result != APP_MSG_OK) {
    // The outbox cannot be used right now
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the outbox: %d", (int)result);
    return;
  }
  
  // Construct the message with a dummy value
  int value = 0;
  // Add an item to ask for weather data
  dict_write_int(out_iter, MESSAGE_KEY_RequestData, &value, sizeof(int), true);
  
  // APP_LOG(APP_LOG_LEVEL_ERROR, "Sending RequestUpdate message for weather...");
  
  // Send this message
  result = app_message_outbox_send();

  // Check the result
  if(result != APP_MSG_OK) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result);
  }
}

static void on_scheduled_update_triggered() {
  // APP_LOG(APP_LOG_LEVEL_DEBUG, "scheduled weather update triggered!");
  
  if (s_update_timer) {
    // cancel weather update timer
    app_timer_cancel(s_update_timer);
  }
  
  // send AppMessage to trigger weather update via JS
  request_weather_update();
  
  //Register next execution
  s_update_timer = app_timer_register(s_weather_update_interval, (AppTimerCallback) on_scheduled_update_triggered, NULL);
}

void update_weather(){
  // APP_LOG(APP_LOG_LEVEL_DEBUG, "updating weather with new data");
  
  // persist current data for fast access when opening the watchface
  save_current_weather_data();
  
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
    replace_gbitmap_color(GColorBlack, theme_get_theme()->WeatherIconColor, s_weather_icon, NULL);
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
  bitmap_layer_set_background_color(s_weather_icon_layer, GColorClear);
  
  s_settings = clay_get_settings();
  
  restore_saved_weather_data();
  
  // set bounds and offset for text layer
  int width = 60;
  int height = 40;
  int offsetX = icon_bounds.size.w + 5 + 5;
  int offsetY = bounds.size.h - height - 5;
  
  GRect layer_bounds = GRect(offsetX, offsetY, width, height);
  
  // Create temperature Layer
  s_weather_layer = text_layer_create(layer_bounds);

  // Style the text
  text_layer_set_background_color(s_weather_layer, theme_get_theme()->BackgroundColor);
  text_layer_set_text_color(s_weather_layer, theme_get_theme()->WeatherTextColor);
  text_layer_set_font(s_weather_layer, theme_get_theme()->WeatherFont);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentLeft);
  text_layer_set_text(s_weather_layer, "---");
  
  update_weather();
  
  s_update_timer = app_timer_register(s_weather_update_interval, (AppTimerCallback) on_scheduled_update_triggered, NULL);
  
  layer_add_child(window_layer, bitmap_layer_get_layer(s_weather_icon_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_weather_layer));
}

void destroy_weather_layer(){
  if (s_update_timer) {
    // cancel weather update timer
    app_timer_cancel(s_update_timer);
  }
  
  // destroy current weather icon
  gbitmap_destroy(s_weather_icon);
  
  // destroy weather icon layer
  bitmap_layer_destroy(s_weather_icon_layer);
  
  // destroy weather temperature layer
  text_layer_destroy(s_weather_layer);
}