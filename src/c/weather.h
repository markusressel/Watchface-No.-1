#pragma once
#include <pebble.h>

// #ifndef EXTERN
// #define EXTERN extern
// #endif

typedef struct WeatherData {
  int CurrentTemperature;
  char* CurrentConditions;
} __attribute__((__packed__)) WeatherData;

// EXTERN int currentTemperature;
// EXTERN char *currentConditions;

WeatherData* weather_get_data();

// update layer information
void update_weather();

// create the layer
void create_weather_layer(Window *window);

// destroy the layer
void destroy_weather_layer();