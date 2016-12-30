#pragma once
#include <pebble.h>

// update layer information
void update_battery_text();

// create the layer
void create_battery_text_layer(Window *window);

// destroy the layer
void destroy_battery_text_layer();