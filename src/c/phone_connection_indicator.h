#pragma once
#include <pebble.h>

// update layer information
void update_phone_connection_indicator();

// create the layer
void create_phone_connection_indicator_layer(Window *window);

// destroy the layer
void destroy_phone_connection_indicator_layer();