#include <pebble.h>
#include "heartrate.h"
#include "health_listener.h"
#include <pebble-gbitmap-lib/gbitmap_tools.h>
#include "clay_settings.h"
#include "theme.h"
#include "gbitmap_color_palette_manipulator.h"

static ClaySettings *s_settings;

// Heartrate TextLayer
static TextLayer *s_heartrate_layer;

// Heart icon Bitmap
static GBitmap *s_heart_icon;
static GBitmap *s_resized_heart_icon;

// Icon layer
static int s_current_heartrate;
static Layer *s_heart_icon_layer;

// number of heartbeat animation images
#define NB_OF_IMAGES 1

// Heartbeat animation callbacks
static AnimationImplementation inAnimImpl;
static AnimationImplementation outAnimImpl;

// Heartbeat animation sequence
static Animation *s_sequence;
// Animation array
static Animation **s_arr;

// heartbeat animation durations (in + out = one cycle)
static int s_heartbeat_animation_out_duration = 225;
static int s_heartbeat_animation_in_duration = 150;
// delay between a full heartbeat animation cycle
static int s_heartbeat_animation_delay = 625;
static const int s_heartbeat_animation_repeat_count = 10;
// static const int s_heartbeat_animation_repeat_count = ANIMATION_DURATION_INFINITE;
static const int s_heartbeat_icon_animation_scale_big = 90;
static const int s_heartbeat_icon_animation_scale_small = 70;
// this has to be |scale_big - scale_small|
static const int s_heartbeat_icon_animation_scale_diff = 20;

static int s_current_heartbeat_icon_scale = 100;

static void do_resize(int percent){
  if (percent == s_current_heartbeat_icon_scale) {
    // dont redraw if the scale didnt change
    return;
  } else {
    // update current scale
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "resizing from %d to %d", s_current_heartbeat_icon_scale, percent);
    s_current_heartbeat_icon_scale = percent;
  }
  
  // rescale and rerender icon
  if(s_resized_heart_icon){
    gbitmap_destroy(s_resized_heart_icon);
  }
  s_resized_heart_icon = scaleBitmap(s_heart_icon, percent, percent);
  layer_mark_dirty(s_heart_icon_layer);
}

static void inAnimationUpdate(struct Animation *animation, const AnimationProgress progress){
  int progressPercent = (progress * 100 / ANIMATION_NORMALIZED_MAX);
  int imageScale = s_heartbeat_icon_animation_scale_big - (progressPercent * s_heartbeat_icon_animation_scale_diff / 100);
  
  do_resize(imageScale);
}

static void outAnimationUpdate(struct Animation *animation, const AnimationProgress progress){
  int progressPercent = (progress * 100 / ANIMATION_NORMALIZED_MAX);  
  int imageScale = s_heartbeat_icon_animation_scale_small + (progressPercent * s_heartbeat_icon_animation_scale_diff / 100);
  
  do_resize(imageScale);
}

/*
static void animationStartedHandler(Animation *animation, void *context){
}

static void animationStoppedHandler(Animation *animation, bool finished, void *context){
}
*/

static void heart_icon_update_callback(Layer *layer, GContext* ctx) {
  if(!s_resized_heart_icon) {
    // create to start size resized icon as default
    s_resized_heart_icon = scaleBitmap(s_heart_icon, 
                                       s_heartbeat_icon_animation_scale_big, 
                                       s_heartbeat_icon_animation_scale_big);;
  }
  
  GRect layer_bounds = layer_get_bounds(layer);
  
  if(s_resized_heart_icon) {
    GRect bounds = gbitmap_get_bounds(s_resized_heart_icon);
    
    // Set the compositing mode (GCompOpSet is required for transparency)
    graphics_context_set_compositing_mode(ctx, GCompOpSet);
    
    graphics_draw_bitmap_in_rect(ctx,
                                 s_resized_heart_icon, 
                                 (GRect) {
                                   .origin = (GPoint) {
                                     (layer_bounds.size.w - bounds.size.w)/2,
                                     (layer_bounds.size.h - bounds.size.h)/2
                                   },
                                     .size=bounds.size
                                 }
                                );
  }
}

static void destroy_heart_animation() {
  // destroy animation related data
  for(uint8_t i=0; i<NB_OF_IMAGES; i++){
    animation_destroy(s_arr[2*i]);
    animation_destroy(s_arr[2*i+1]);
  }
  
  animation_destroy(s_sequence);
  // Destroy the array
  free(s_arr);
}

static void initialize_heart_animation() {
  inAnimImpl.update = inAnimationUpdate;
  outAnimImpl.update = outAnimationUpdate;
  
  // destroy existing animation if necessary
  if (s_arr) {
    destroy_heart_animation();
  }
  
  s_arr = (Animation**)malloc((2 * NB_OF_IMAGES) * sizeof(Animation*));
  for(uint8_t i=0; i<NB_OF_IMAGES; i++){
    Animation *animation=animation_create();
    animation_set_duration(animation, s_heartbeat_animation_in_duration);
    animation_set_delay(animation, s_heartbeat_animation_delay);
    animation_set_curve(animation, AnimationCurveEaseIn);
    animation_set_implementation(animation, &inAnimImpl);
    //animation_set_handlers(animation,(AnimationHandlers){
    //  .started = animationStartedHandler,
    //  .stopped = animationStoppedHandler
    //}, (void*)(uint32_t)i);
    s_arr[2*i] = animation;

    animation=animation_create();
    animation_set_duration(animation, s_heartbeat_animation_out_duration);
    animation_set_curve(animation, AnimationCurveEaseOut);
    animation_set_implementation(animation, &outAnimImpl);
    s_arr[2*i+1] = animation;
  }

  // Create the sequence, set to loop forever
  s_sequence = animation_sequence_create_from_array(s_arr, 2 * NB_OF_IMAGES);
  animation_set_play_count(s_sequence, s_heartbeat_animation_repeat_count);
}

// update heart animation properties if BPM changed
static void update_heart_animation() {
  initialize_heart_animation();
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "rescheduling heart animation");
  animation_schedule(s_sequence);
}

// Method to update the heartrate textbuffer
void update_heartrate() {
  // fake heartrate for testing
  s_heartrate_bpm = 90;
  
  if (s_current_heartrate != s_heartrate_bpm) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "heartrate changed: new %d, old: %d", s_heartrate_bpm, s_current_heartrate);
    s_current_heartrate = s_heartrate_bpm;
    
    if (s_heartrate_bpm != 0) {
      // calculate animation speed based on new heartrate
      s_heartbeat_animation_delay = (int) 
        ((float) 60 / 
         s_heartrate_bpm * 1000 - 
         (s_heartbeat_animation_in_duration + s_heartbeat_animation_out_duration));
      
      APP_LOG(APP_LOG_LEVEL_DEBUG, 
              "animation speed is now %d + %d + %d", 
              s_heartbeat_animation_out_duration, 
              s_heartbeat_animation_in_duration,
              s_heartbeat_animation_delay);
  
      // set to 0 if smaller than 0 (BPM > 160) with current values (375ms heartbeat animation)
      if (s_heartbeat_animation_delay < 0) {
        s_heartbeat_animation_delay = 0;
      }
      
      update_heart_animation();
    } else {
      // set speed to default value but DONT start animation
      s_heartbeat_animation_delay = 625; // 60 BPM
    }
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "ignoring already visble heartrate update");
  }
  
  
  // Write the current heartrate into a buffer
  static char s_buffer[8];
  snprintf(s_buffer, sizeof(s_buffer), "%d", (int)s_heartrate_bpm);
  
  // Display this heartrate on the TextLayer
  text_layer_set_text(s_heartrate_layer, s_buffer);
}

// creates the layer
void create_heartrate_layer(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  s_settings = clay_get_settings();
  
  int icon_width = 50;
  int icon_height = 50;
  int icon_offsetX = bounds.size.w - icon_width;
  int icon_offsetY = bounds.size.h - icon_height;
  
  GRect icon_bounds = GRect(icon_offsetX, icon_offsetY, icon_width, icon_height);
  
  // load heart icon
  s_heart_icon = gbitmap_create_with_resource(RESOURCE_ID_HEARTRATE_ICON_HEART_FILLED);
//  #ifdef PBL_COLOR
  replace_gbitmap_color(GColorBlack, theme_get_theme()->HeartIconColor, s_heart_icon, NULL);
  //replace_gbitmap_color(GColorBlack, GColorRed, s_heart_icon, NULL);
//  #endif
  
  s_heart_icon_layer = layer_create(icon_bounds);
  layer_set_update_proc(s_heart_icon_layer, heart_icon_update_callback);
  
  // initialize animation
  initialize_heart_animation();
  
  int width = 50;
  int height = 20;
  int offsetX = bounds.size.w - (icon_bounds.size.w / 2) - (width / 2);
  int offsetY = bounds.size.h - icon_bounds.size.h + 11;
  
  GRect layer_bounds = GRect(offsetX, offsetY, width, height);
  
  // Create the TextLayer with specific bounds
  s_heartrate_layer = text_layer_create(layer_bounds);
  
  // set styling
  text_layer_set_background_color(s_heartrate_layer, GColorClear);
  text_layer_set_text_color(s_heartrate_layer, theme_get_theme()->HeartrateTextColor);
  text_layer_set_font(s_heartrate_layer, theme_get_theme()->HeartrateFont);
  text_layer_set_text_alignment(s_heartrate_layer, GTextAlignmentCenter);

  // update value before rendering so it is shown right from the beginning
  s_heartrate_bpm = health_service_peek_current_value(HealthMetricHeartRateBPM);
  update_heartrate();
  
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, s_heart_icon_layer);
  layer_add_child(window_layer, text_layer_get_layer(s_heartrate_layer));
}

// destroys the layer
void destroy_heartrate_layer() {
  // destroy icon
  if(s_heart_icon) {
    gbitmap_destroy(s_heart_icon);
  }
  // destroy any remaining resized icon
  if(s_resized_heart_icon) {
    gbitmap_destroy(s_resized_heart_icon);
  }
  
  // destroy layers
  layer_destroy(s_heart_icon_layer);
  text_layer_destroy(s_heartrate_layer);
  
  // destroy animation related data
  destroy_heart_animation();
}