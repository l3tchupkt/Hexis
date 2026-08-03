#include "hexis.h"
#include "../drivers/ch341a/ch341a.h"

int hexis_init(void) {
    // Register all standard drivers
    hexis_ch341a_register();
    return 0;
}

const char* hexis_get_version(void) {
    return "1.0.0";
}
