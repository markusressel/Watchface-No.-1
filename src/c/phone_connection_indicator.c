#include <pebble.h>
#include "phone_connection_indicator.h"
#include "theme.h"

// phone connection indicator layer
static Layer *s_phone_connection_indicator_layer;

static bool is_phone_app_connected;

static ConnectionHandler connectionHandler = NULL;

static void connection_changed(bool connected) {
  is_phone_app_connected = connected;
  
  update_phone_connection_indicator();
}

static void phone_connection_indicator_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  
  if (is_phone_app_connected) {
    // Fill it completely
    graphics_context_set_fill_color(ctx, foregroundColor);
    graphics_fill_rect(ctx, GRect(0, 0, bounds.size.w, bounds.size.h), 0, GCornerNone);
  } else {
    // Dont fill
    graphics_context_set_stroke_color(ctx, foregroundColor);
    graphics_draw_rect(ctx, GRect(0, 0, bounds.size.w, bounds.size.h));
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
  GRect bounds = layer_get_unobstructed_bounds(window_layer);
  
  // Create battery meter Layer
  int width = 10;
  int height = 10;
  int offsetX = 5;
  int offsetY = 5;
  GRect layer_bounds = GRect(offsetX, offsetY, width, height);
  s_phone_connection_indicator_layer = layer_create(layer_bounds);
  layer_set_update_proc(s_phone_connection_indicator_layer, phone_connection_indicator_update_proc);  
  
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = connectionHandler
  });
  // force update
  update_phone_connection_indicator();
  
  // Add to Window
  layer_add_child(window_get_root_layer(window), s_phone_connection_indicator_layer);
}

// destroy the layer
void destroy_phone_connection_indicator_layer() {
  connection_service_unsubscribe();
  
  layer_destroy(s_phone_connection_indicator_layer);
}