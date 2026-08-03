#include <hexis_plugin.h>
#include <stdio.h>

static int my_plugin_init(void) {
    printf("[ExamplePlugin] Initializing...\n");
    return 0;
}

static void my_plugin_cleanup(void) {
    printf("[ExamplePlugin] Cleaning up...\n");
}

static void my_plugin_run(void) {
    printf("[ExamplePlugin] Running analysis!\n");
}

static hexis_plugin_t my_plugin = {
    .name = "Example Hexis Plugin",
    .version = "1.0.0",
    .author = "Letchu",
    .description = "A demonstration of the Hexis dynamic plugin system.",
    .init = my_plugin_init,
    .cleanup = my_plugin_cleanup,
    .run = my_plugin_run
};

#ifdef _WIN32
__declspec(dllexport) hexis_plugin_t* hexis_plugin_entry(void) {
#else
hexis_plugin_t* hexis_plugin_entry(void) {
#endif
    return &my_plugin;
}
