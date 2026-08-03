#include "hexis_event.h"
#include <stdlib.h>
#include <string.h>

#define MAX_SUBSCRIBERS 128
#define MAX_EVENT_TYPES 20

typedef struct {
    hexis_event_callback_t cb;
    void* user_data;
} hexis_subscriber_t;

static hexis_subscriber_t g_subscribers[MAX_EVENT_TYPES][MAX_SUBSCRIBERS];
static size_t g_sub_counts[MAX_EVENT_TYPES];

int hexis_event_subscribe(hexis_event_type_t type, hexis_event_callback_t cb, void* user_data) {
    if (type >= MAX_EVENT_TYPES || !cb) return -1;
    
    size_t count = g_sub_counts[type];
    if (count >= MAX_SUBSCRIBERS) return -1;
    
    // Avoid duplicates
    for (size_t i = 0; i < count; ++i) {
        if (g_subscribers[type][i].cb == cb && g_subscribers[type][i].user_data == user_data) {
            return 0; // Already subscribed
        }
    }
    
    g_subscribers[type][count].cb = cb;
    g_subscribers[type][count].user_data = user_data;
    g_sub_counts[type]++;
    
    return 0;
}

int hexis_event_unsubscribe(hexis_event_type_t type, hexis_event_callback_t cb) {
    if (type >= MAX_EVENT_TYPES || !cb) return -1;
    
    size_t count = g_sub_counts[type];
    for (size_t i = 0; i < count; ++i) {
        if (g_subscribers[type][i].cb == cb) {
            // Shift remaining elements
            for (size_t j = i; j < count - 1; ++j) {
                g_subscribers[type][j] = g_subscribers[type][j + 1];
            }
            g_sub_counts[type]--;
            return 0;
        }
    }
    return -1;
}

void hexis_event_publish(const hexis_event_t* event) {
    if (!event || event->type >= MAX_EVENT_TYPES) return;
    
    size_t count = g_sub_counts[event->type];
    for (size_t i = 0; i < count; ++i) {
        g_subscribers[event->type][i].cb(event, g_subscribers[event->type][i].user_data);
    }
}
