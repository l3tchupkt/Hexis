#ifndef HEXIS_EVENT_H
#define HEXIS_EVENT_H

#include "hexis.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HEXIS_EVENT_PROGRAMMER_CONNECTED,
    HEXIS_EVENT_DRIVER_LOADED,
    HEXIS_EVENT_CHIP_IDENTIFIED,
    HEXIS_EVENT_FIRMWARE_READ,
    HEXIS_EVENT_PARSER_STARTED,
    HEXIS_EVENT_SECRETS_FOUND,
    HEXIS_EVENT_REPORT_GENERATED
} hexis_event_type_t;

typedef struct {
    hexis_event_type_t type;
    void* payload; // Type-specific payload (e.g. HexisFirmware*)
    const char* sender_module;
} hexis_event_t;

typedef void (*hexis_event_callback_t)(const hexis_event_t* event, void* user_data);

HEXIS_API int hexis_event_subscribe(hexis_event_type_t type, hexis_event_callback_t cb, void* user_data);
HEXIS_API int hexis_event_unsubscribe(hexis_event_type_t type, hexis_event_callback_t cb);
HEXIS_API void hexis_event_publish(const hexis_event_t* event);

#ifdef __cplusplus
}
#endif

#endif // HEXIS_EVENT_H
