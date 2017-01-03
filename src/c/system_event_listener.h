#pragma once

#ifndef EXTERN
#define EXTERN extern
#endif

// phone app connection status
EXTERN bool is_phone_app_connected;

// register a single system event listener for the application
// there can only be one single callback
void register_system_event_listener();

// unregister the system event listener
void unregister_system_event_listener();