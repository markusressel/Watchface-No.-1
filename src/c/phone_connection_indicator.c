#include <pebble.h>
#include "phone_connection_indicator.h"
#include "system_event_listener.h"
#include <pebble-effect-layer/pebble-effect-layer.h>
#include <pebble-gbitmap-lib/gbitmap_tools.h>
#include "clay_settings.h"

static ClaySettings *s_settings;

// phone connection indicator layer
static Layer *s_phone_connection_indicator_layer;

static GBitmap *s_connection_icon;

// Effect layer, covering the icon to change image color
static EffectLayer *s_effect_layer;

static void phone_connection_indicator_update_proc(Layer *layer, GContext *ctx) {
  GRect layer_bounds = layer_get_bounds(layer);
  
  // draw background color (for correct color inversion)
  graphics_context_set_fill_color(ctx, GColorWhite);
  GRect bounds = layer_get_bounds(layer);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);
  
  if (s_connection_icon) {
    // Set the compositing mode (GCompOpSet is required for transparency)
    graphics_context_set_compositing_mode(ctx, GCompOpSet);
    
    GRect bounds = gbitmap_get_bounds(s_connection_icon);
    
    graphics_draw_bitmap_in_rect(
      ctx,
      s_connection_icon, 
      (GRect) {
        .origin = (GPoint) {
          (layer_bounds.size.w - bounds.size.w)/2 - 3,
          (layer_bounds.size.h - bounds.size.h)/2
        },
      .size=bounds.size
      }
    );                   
  }
  
  if (!is_phone_app_connected) {
    graphics_context_set_stroke_color(ctx, GColorBlack);
    graphics_context_set_stroke_width(ctx, 5);
    
    // draw cross over icon
    graphics_draw_line(ctx, GPoint(5,5), GPoint(layer_bounds.size.w - 5, layer_bounds.size.h - 5));
    graphics_draw_line(ctx, GPoint(5,layer_bounds.size.h - 5), GPoint(layer_bounds.size.w - 5, 5));
  }
}

// update layer information
void update_phone_connection_indicator() {
  is_phone_app_connected = connection_service_peek_pebble_app_connection();
  layer_mark_dirty(s_phone_connection_indicator_layer);
}

// create the layer
void create_phone_connection_indicator_layer(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  // GRect bounds = layer_get_bounds(window_layer);
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "retreiving settings");
  s_settings = clay_get_settings();
  
  // Create battery meter Layer
  int width = 25;
  int height = 25;
  int offsetX = 0;
  int offsetY = 0;
  GRect layer_bounds = GRect(offsetX, offsetY, width, height);
  s_phone_connection_indicator_layer = layer_create(layer_bounds);
  layer_set_update_proc(s_phone_connection_indicator_layer, phone_connection_indicator_update_proc);  
  
  s_connection_icon = gbitmap_create_with_resource(RESOURCE_ID_PHONE_CONNECTION_ICON);
  s_connection_icon = scaleBitmap(s_connection_icon, 40, 40);
  
  // Create effect layer
  s_effect_layer = effect_layer_create(layer_bounds);
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "checking ThemeValue");
  if (strcmp(s_settings->ThemeValue, "LIGHT") != 0) {
    // add color inversion effect
    effect_layer_add_effect(s_effect_layer, effect_invert, NULL);
  }
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "updating indicator");
  // force update
  update_phone_connection_indicator();
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "adding to parent");
  // Add to Window
  layer_add_child(window_get_root_layer(window), s_phone_connection_indicator_layer);
  layer_add_child(window_layer, effect_layer_get_layer(s_effect_layer));
}

// destroy the layer
void destroy_phone_connection_indicator_layer() {
  // destroy icon
  if(s_connection_icon) {
    gbitmap_destroy(s_connection_icon);
  }
  
  effect_layer_destroy(s_effect_layer);
  layer_destroy(s_phone_connection_indicator_layer);
}