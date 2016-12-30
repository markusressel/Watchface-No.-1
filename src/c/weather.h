#pragma once
#include <pebble.h>

// update layer information
void update_weather();

// create the layer
void create_weather_layer(Window *window);

// destroy the layer
void destroy_weather_layer();