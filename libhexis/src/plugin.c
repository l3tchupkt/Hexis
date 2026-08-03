#include "../include/hexis_plugin.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#define MAX_PLUGINS 32

static hexis_plugin_t* active_plugins[MAX_PLUGINS];
static int plugin_count = 0;

int hexis_plugin_load(const char* path) {
    if (!path) return -1;
    if (plugin_count >= MAX_PLUGINS) return -2;

#ifdef _WIN32
    HMODULE handle = LoadLibraryA(path);
    if (!handle) {
        fprintf(stderr, "Failed to load plugin: %s\n", path);
        return -3;
    }
    
    hexis_plugin_entry_func entry = (hexis_plugin_entry_func)GetProcAddress(handle, "hexis_plugin_entry");
    if (!entry) {
        fprintf(stderr, "Plugin missing entry point: %s\n", path);
        FreeLibrary(handle);
        return -4;
    }
#else
    void* handle = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
    if (!handle) {
        fprintf(stderr, "Failed to load plugin: %s (%s)\n", path, dlerror());
        return -3;
    }
    
    union {
        void* ptr;
        hexis_plugin_entry_func func;
    } cast_u;
    cast_u.ptr = dlsym(handle, "hexis_plugin_entry");
    hexis_plugin_entry_func entry = cast_u.func;
    if (!entry) {
        fprintf(stderr, "Plugin missing entry point: %s (%s)\n", path, dlerror());
        dlclose(handle);
        return -4;
    }
#endif

    hexis_plugin_t* plugin = entry();
    if (!plugin) {
        fprintf(stderr, "Plugin returned null on initialization.\n");
        return -5;
    }

    if (plugin->init) {
        if (plugin->init() != 0) {
            fprintf(stderr, "Plugin %s failed to initialize.\n", plugin->name);
            return -6;
        }
    }

    active_plugins[plugin_count++] = plugin;
    printf("Loaded Hexis Plugin: %s v%s by %s\n", plugin->name, plugin->version, plugin->author);
    
    return 0;
}
