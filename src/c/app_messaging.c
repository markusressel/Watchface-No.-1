#include <pebble.h>
#include "app_messaging.h"
#include "weather.h"
#include "clay_settings.h"

static ClaySettings *settings;

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read clay configuration properties
  
  settings = clay_get_settings();
  
  // Read color preferences
  Tuple *bg_color_t = dict_find(iterator, MESSAGE_KEY_BackgroundColor);
  if(bg_color_t) {
    GColor bg_color = GColorFromHEX(bg_color_t->value->int32);
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "bg_color: old: %lu new: %lu", settings.BackgroundColor, bg_color);
    settings->BackgroundColor = bg_color;
  }

  Tuple *fg_color_t = dict_find(iterator, MESSAGE_KEY_ForegroundColor);
  if(fg_color_t) {
    GColor fg_color = GColorFromHEX(fg_color_t->value->int32);
    settings->ForegroundColor = fg_color;
  }
  
  Tuple *text_color_t = dict_find(iterator, MESSAGE_KEY_TextColor);
  if(text_color_t) {
    GColor text_color = GColorFromHEX(text_color_t->value->int32);
    settings->TextColor = text_color;
  }

  // Read boolean preferences
  Tuple *show_seconds_t = dict_find(iterator, MESSAGE_KEY_ShowSeconds);
  if(show_seconds_t) {
    bool show_seconds = show_seconds_t->value->int32 == 1;
    settings->ShowSeconds = show_seconds;
  }
  
  Tuple *show_animations_t = dict_find(iterator, MESSAGE_KEY_ShowAnimations);
  if(show_animations_t) {
    bool show_animations = show_animations_t->value->int32 == 1;
    settings->ShowAnimations = show_animations;
  }
  
  // Read weather data
  
  // Read tuples for data
  Tuple *temp_tuple = dict_find(iterator, MESSAGE_KEY_WEATHER_TEMPERATURE);
  Tuple *condition_tuple = dict_find(iterator, MESSAGE_KEY_WEATHER_CONDITION);
  
  // If all data is available, use it
  if(temp_tuple && condition_tuple) {
    WeatherData *weatherData = weather_get_data();
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "temp changed: old: %d new: %d",(int)  weatherData->CurrentTemperature, (int)temp_tuple->value->int32);
    
    weatherData->CurrentTemperature = (int)temp_tuple->value->int32;
    weatherData->CurrentConditions = condition_tuple->value->cstring;
    
    // snprintf(s_temperature_buffer, sizeof(s_temperature_buffer), "%d°C", (int)temp_tuple->value->int32);
    // snprintf(s_conditions_buffer, sizeof(s_conditions_buffer), "%s", condition_tuple->value->cstring);
    
    update_weather();
  }
  
  // persist data
  clay_save_settings();
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void initialize_app_messaging() {
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  const int inbox_size = 128;
  const int outbox_size = 128;
  app_message_open(inbox_size, outbox_size);
}