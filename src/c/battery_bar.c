#include "battery_bar.h"
#include "battery.h"
#include "theme.h"
#include "battery_listener.h"

// battery bar layer
static Layer *s_battery_bar_layer;

// layer dimensions and positioning
static int layer_width;
static int height;
static int offsetX;
static int offsetY;

// draw the battery layer
static void battery_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  
  // Draw battery icon rectangle
  GRect batteryBorder = bounds;
  batteryBorder.size.w -= 2;
  graphics_context_set_stroke_color(ctx, foregroundColor);
  graphics_draw_rect(ctx, batteryBorder);
  
  // Draw battery +(plus)-pole
  int poleHeight = bounds.size.h * 0.5;
  int poleWidth = 2;
  GRect batteryPole = GRect(bounds.size.w - poleWidth, bounds.size.h / 2 - poleHeight / 2, poleWidth, poleHeight);
  graphics_context_set_fill_color(ctx, foregroundColor);
  graphics_fill_rect(ctx, batteryPole, 0, GCornerNone);

  int insetX = 2;
  int insetY = 2;
  if (s_battery_cable_connected) {
    // Draw cable
    graphics_context_set_stroke_width(ctx, 1);
    graphics_draw_line(ctx, GPoint(0 ,bounds.size.h / 2), GPoint(bounds.size.w ,bounds.size.h / 2));
    
    // Draw the bar inside
    if (s_battery_charging) {
      // TODO: charging animation
    } else {
      // Fill it completely
      int fillWidth = layer_width - insetY * 2 - poleWidth;
      graphics_context_set_fill_color(ctx, foregroundColor);
      graphics_fill_rect(ctx, GRect(insetX, insetY, fillWidth, bounds.size.h - insetX * 2), 0, GCornerNone);
    }
  } else {
    // Draw the bar inside
    int fillWidth = (s_battery_level * (layer_width - insetY * 2 - poleWidth)) / 100;
    graphics_context_set_fill_color(ctx, foregroundColor);
    graphics_fill_rect(ctx, GRect(insetX, insetY, fillWidth, bounds.size.h - insetX * 2), 0, GCornerNone);
  }
}

void update_battery_bar(){
  // Update meter
  layer_mark_dirty(s_battery_bar_layer);
}

void create_battery_bar_layer(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_unobstructed_bounds(window_layer);
  
  // Create battery meter Layer
  layer_width = 50;
  height = 15;
  offsetX = (bounds.size.w - layer_width) / 2; // centered
  offsetY = 5;
  GRect layer_bounds = GRect(offsetX, offsetY, layer_width, height);
  s_battery_bar_layer = layer_create(layer_bounds);
  layer_set_update_proc(s_battery_bar_layer, battery_update_proc);  
  
  // Add to Window
  layer_add_child(window_get_root_layer(window), s_battery_bar_layer);
}

void destroy_battery_bar_layer(){
  layer_destroy(s_battery_bar_layer);
}