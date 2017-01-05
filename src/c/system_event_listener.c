#include <pebble.h>
#define EXTERN
#include "system_event_listener.h"

#include "phone_connection_indicator.h"

static void connection_changed(bool connected) {
  is_phone_app_connected = connected;
  
  update_phone_connection_indicator();
}

void register_system_event_listener() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "registering system event listener");
  
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = connection_changed
  });
}

void unregister_system_event_listener() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "unregistering system event listener");
  
  connection_service_unsubscribe();
}