#include <pebble.h>
#define EXTERN
#include "battery.h"
#include "battery_listener.h"
#include "battery_bar.h"
#include "battery_text.h"

static bool registered = false;

static void battery_callback(BatteryChargeState state) {
  // Record the new battery level
  s_battery_level = state.charge_percent;
  s_battery_charging = state.is_charging;
  s_battery_cable_connected = state.is_plugged;
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Battery changed: %d", (int) s_battery_level);
  
  update_battery_text();
  update_battery_bar();
}

void register_battery_listener() {
  if (registered) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "ignoring register battery listener");
    return;
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "registering battery listener");
  
  battery_state_service_subscribe(battery_callback);
  registered = true;
}

void unregister_battery_listener() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "unregistering battery listener");
  
  battery_state_service_unsubscribe();
  registered = false;
}

void force_battery_update() {
  battery_callback(battery_state_service_peek());
}