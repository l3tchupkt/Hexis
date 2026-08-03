#include "hexis_driver.h"
#include <string.h>

#define MAX_DRIVERS 32

static const hexis_driver_t* registered_drivers[MAX_DRIVERS];
static size_t num_drivers = 0;

int hexis_driver_register(const hexis_driver_t* driver) {
    if (!driver || !driver->name) return -1;
    if (num_drivers >= MAX_DRIVERS) return -2; // Too many drivers
    
    // Check if already registered
    for (size_t i = 0; i < num_drivers; ++i) {
        if (strcmp(registered_drivers[i]->name, driver->name) == 0) {
            return -3; // Already registered
        }
    }
    
    registered_drivers[num_drivers++] = driver;
    return 0;
}

const hexis_driver_t* hexis_driver_get(const char* name) {
    if (!name) return nullptr;
    
    for (size_t i = 0; i < num_drivers; ++i) {
        if (strcmp(registered_drivers[i]->name, name) == 0) {
            return registered_drivers[i];
        }
    }
    return nullptr;
}
