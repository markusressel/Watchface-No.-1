#include "battery_bar.h"
#include "battery.h"
#include "theme.h"
#include "battery_listener.h"

// battery bar layer
static Layer *s_battery_bar_layer;

static GPath *s_my_path_ptr = NULL;

static const GPathInfo BOLT_PATH_INFO = {
  .num_points = 6,
  .points = (GPoint []) {{4, 0}, {3, 4}, {6, 4}, {2, 9}, {3, 5}, {0, 5}}
};

// temporary variable used for animation
static int s_current_battery_level;

// battery charging animation callbacks
static AnimationImplementation batteryChargingAnimImpl;

static Animation *charging_animation;

// battery charging animation durations
static int s_battery_charging_animation_duration = 2000;
// delay between a full battery charging animation cycle
static int s_battery_charging_animation_delay = 0;
static const int s_battery_charging_animation_repeat_count = ANIMATION_DURATION_INFINITE;

// layer dimensions and positioning
static int layer_width;
static int height;
static int offsetX;
static int offsetY;

static void draw_battery_fill(int percent) {
  s_current_battery_level = percent;
  layer_mark_dirty(s_battery_bar_layer);
}

// update battery charging animation
static void batteryChargingAnimUpdate(struct Animation *animation, const AnimationProgress progress){
  int progressPercent = (progress * 100 / ANIMATION_NORMALIZED_MAX);
  // calculate animation fill rate
  int fillPercent = s_battery_level + (progressPercent * (100 - s_battery_level) / 100);
  
  draw_battery_fill(fillPercent);
}


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
  
  // Draw the bar inside
  int fillWidth = (s_current_battery_level * (layer_width - insetY * 2 - poleWidth)) / 100;
  graphics_context_set_fill_color(ctx, foregroundColor);
  graphics_fill_rect(ctx, GRect(insetX, insetY, fillWidth, bounds.size.h - insetX * 2), 0, GCornerNone);
  
  if (s_battery_cable_connected) {
    // Draw cable
    
    // Fill the path:
    graphics_context_set_fill_color(ctx, backgroundColor);
    gpath_draw_filled(ctx, s_my_path_ptr);
    // Stroke the path:
    graphics_context_set_stroke_width(ctx, 1);
    graphics_context_set_stroke_color(ctx, backgroundColor);
    gpath_draw_outline(ctx, s_my_path_ptr);
    
   // graphics_draw_line(ctx, GPoint(0 ,bounds.size.h / 2), GPoint(bounds.size.w ,bounds.size.h / 2));
  } 
}

static void destroy_battery_charging_animation() {
  animation_unschedule(charging_animation);
  
  // destroy animation related data
  animation_destroy(charging_animation);
}

static void initialize_battery_charging_animation() {
  batteryChargingAnimImpl.update = batteryChargingAnimUpdate;
  
  charging_animation = animation_create();
  animation_set_duration(charging_animation, s_battery_charging_animation_duration);
  animation_set_delay(charging_animation, s_battery_charging_animation_delay);
  animation_set_curve(charging_animation, AnimationCurveEaseInOut);
  animation_set_implementation(charging_animation, &batteryChargingAnimImpl);
  animation_set_play_count(charging_animation, s_battery_charging_animation_repeat_count);
  //animation_set_handlers(animation,(AnimationHandlers){
  //  .started = animationStartedHandler,
  //  .stopped = animationStoppedHandler
  //}, (void*)(uint32_t)i);
  
  animation_schedule(charging_animation);
}


void update_battery_bar(){
  if (s_battery_charging) {
    if (!charging_animation) {
      initialize_battery_charging_animation();
    }
  } else {
    // disable charging animation, if it was active before
    if (charging_animation) {
      destroy_battery_charging_animation();
    }
  }
  
  // update current battery level
  s_current_battery_level = s_battery_level;
  
  // Update meter (and redraw)
  layer_mark_dirty(s_battery_bar_layer);
}

void create_battery_bar_layer(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // Create battery meter Layer
  layer_width = 50;
  height = 16;
  offsetX = (bounds.size.w - layer_width) / 2; // centered
  offsetY = 5;
  GRect layer_bounds = GRect(offsetX, offsetY, layer_width, height);
  
  // initialize charging icon path
  s_my_path_ptr = gpath_create(&BOLT_PATH_INFO);
  // Rotate 15 degrees:
  // gpath_rotate_to(s_my_path_ptr, TRIG_MAX_ANGLE / 360 * 15);
  // Translate (move) to other layer components position
  gpath_move_to(s_my_path_ptr, GPoint(3, 3));
  
  
  s_battery_bar_layer = layer_create(layer_bounds);
  layer_set_update_proc(s_battery_bar_layer, battery_update_proc);  
  
  // Add to Window
  layer_add_child(window_get_root_layer(window), s_battery_bar_layer);
  
  // update on create
  update_battery_bar();
}

void destroy_battery_bar_layer(){
  gpath_destroy(s_my_path_ptr);
  layer_destroy(s_battery_bar_layer);
  
  if (charging_animation) {
    destroy_battery_charging_animation();
  }
}