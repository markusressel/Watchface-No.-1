#include "weather.h"
#include "theme.h"
#include <pebble-effect-layer/pebble-effect-layer.h>

// Weather TextLayer
static TextLayer *s_weather_layer;

// Weather Font
static GFont s_weather_font;

// Weather icon Bitmap
static GBitmap *s_weather_icon;

// Weather icon PDC
static Layer *s_weather_icon_canvas_layer;
static GDrawCommandImage *s_weather_image_pdc;

// Effect layer, covering the weather icon to change image color
static EffectLayer *s_effect_layer;

// Icon layer
static BitmapLayer *s_weather_icon_layer;

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {

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

static void weather_icon_update_proc(Layer *layer, GContext *ctx) {
  // Place image in the center of the Window
  GSize img_size = gdraw_command_image_get_bounds_size(s_weather_image_pdc);
  GRect bounds = layer_get_bounds(layer);

  const GEdgeInsets frame_insets = {
    .top = (bounds.size.h - img_size.h) / 2,
    .left = (bounds.size.w - img_size.w) / 2
  };

  // If the image was loaded successfully...
  if (s_weather_image_pdc) {
    // Draw it
    gdraw_command_image_draw(ctx, s_weather_image_pdc, grect_inset(bounds, frame_insets).origin);
  } 
  
}

void update_weather(){
  // Write the current temperature into a buffer
  static char s_buffer[8];
  snprintf(s_buffer, sizeof(s_buffer), "33°C");
  // update text layer
  text_layer_set_text(s_weather_layer, s_buffer);
}

static void initialize_app_message() {
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

void create_weather_layer(Window *window){
  initialize_app_message();
  
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  GRect icon_bounds = GRect(5, bounds.size.h - 30, 25, 25);
  
  // Bitmap
  s_weather_icon = gbitmap_create_with_resource(RESOURCE_ID_WEATHER_ICON_SUN);
  s_weather_icon_layer = bitmap_layer_create(icon_bounds);
  bitmap_layer_set_compositing_mode(s_weather_icon_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_weather_icon_layer, s_weather_icon);
  bitmap_layer_set_background_color(s_weather_icon_layer,GColorWhite);
  
  // PDC
  s_weather_icon_canvas_layer = layer_create(icon_bounds);
  layer_set_update_proc(s_weather_icon_canvas_layer, weather_icon_update_proc);
  
  s_weather_image_pdc = gdraw_command_image_create_with_resource(RESOURCE_ID_WEATHER_ICON_SUN_PDC);
  if (!s_weather_image_pdc) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Image is NULL!");
  }
  
 
  s_effect_layer = effect_layer_create(icon_bounds);
  if (appTheme == DARK) {
    //EffectColorpair colorpair_background;
    //colorpair_background.firstColor = GColorWhite;
    //colorpair_background.secondColor = backgroundColor;
    //effect_layer_add_effect(s_effect_layer, effect_colorize, &colorpair_background);
  
    //EffectColorpair colorpair_foreground;
    //colorpair_foreground.firstColor = GColorBlack;
    //colorpair_foreground.secondColor = foregroundColor;
    //effect_layer_add_effect(s_effect_layer, effect_colorize, &colorpair_foreground);
    effect_layer_add_effect(s_effect_layer, effect_invert, NULL);
  }
  
  int width = 60;
  int height = 25;
  int offsetX = icon_bounds.size.w + 5 + 5;
  int offsetY = bounds.size.h - height - 5;
  
  GRect layer_bounds = GRect(offsetX, offsetY, width, height);
  
  // Create temperature Layer
  s_weather_layer = text_layer_create(layer_bounds);

  // Style the text
  text_layer_set_background_color(s_weather_layer, backgroundColor);
  text_layer_set_text_color(s_weather_layer, textColor);
  text_layer_set_font(s_weather_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentLeft);
  text_layer_set_text(s_weather_layer, "Loading...");
  
  //layer_add_child(window_layer, bitmap_layer_get_layer(s_weather_icon_layer));
  layer_add_child(window_layer, s_weather_icon_canvas_layer);
  layer_add_child(window_layer, text_layer_get_layer(s_weather_layer));
  layer_add_child(window_layer, effect_layer_get_layer(s_effect_layer));
}

void destroy_weather_layer(){
  // Destroy canvas Layer
  layer_destroy(s_weather_icon_canvas_layer);
  // Destroy the image
  gdraw_command_image_destroy(s_weather_image_pdc);
  
  gbitmap_destroy(s_weather_icon);
  // bitmap_layer_destroy(s_weather_icon_layer);
  effect_layer_destroy(s_effect_layer);
  
  text_layer_destroy(s_weather_layer);
}