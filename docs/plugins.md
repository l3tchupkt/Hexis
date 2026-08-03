# Developer Guide & Plugin Development

Hexis uses GCC attributes to auto-register plugins.

```c
#include "hexis_parser.h"

__attribute__((constructor))
static void register_my_plugin(void) {
    hexis_parser_t parser = { .name = "MyPlugin" };
    hexis_parser_register(&parser);
}
```
