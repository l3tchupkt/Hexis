#ifndef HEXIS_PLUGIN_H
#define HEXIS_PLUGIN_H

#include "hexis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char* name;
    const char* version;
    const char* author;
    const char* description;
    
    int (*init)(void);
    void (*cleanup)(void);
    void (*run)(void);
} hexis_plugin_t;

/**
 * @brief Every plugin must export this symbol to be recognized by Hexis.
 * 
 * Example:
 * HEXIS_API hexis_plugin_t* hexis_plugin_entry(void) {
 *     return &my_plugin;
 * }
 */
typedef hexis_plugin_t* (*hexis_plugin_entry_func)(void);

/**
 * @brief Load a plugin dynamically.
 * @param path Path to the .so or .dll file.
 * @return 0 on success.
 */
HEXIS_API int hexis_plugin_load(const char* path);

#ifdef __cplusplus
}
#endif

#endif /* HEXIS_PLUGIN_H */
