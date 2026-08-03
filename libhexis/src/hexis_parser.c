#include "hexis_parser.h"
#include <stdio.h>
#include <string.h>

#define MAX_PARSERS 32

static const hexis_parser_t* g_parsers[MAX_PARSERS];
static size_t g_parser_count = 0;

int hexis_parser_register(const hexis_parser_t* parser) {
    if (!parser || g_parser_count >= MAX_PARSERS) {
        return -1;
    }
    g_parsers[g_parser_count++] = parser;
    return 0;
}
