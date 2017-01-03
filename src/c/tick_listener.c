#include <pebble.h>
#include "tick_listener.h"
#include "time.h"
#include "date.h"

static bool registered = false;

// Method to react to tickHandler events (time changes)
static void time_tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  update_date();
}

void register_tick_listener() {
  if (registered) {
    return;
  } 
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, time_tick_handler);
  registered = true;
}

void unregister_tick_listener() {
  // Unregister from TickTimerService
  tick_timer_service_unsubscribe();
}