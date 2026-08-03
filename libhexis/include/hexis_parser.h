#ifndef HEXIS_PARSER_H
#define HEXIS_PARSER_H

#include "hexis_models.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Abstract Parser Plugin Interface.
 * 
 * Native parsers (ELF, SquashFS, uImage) implement this to automatically
 * detect and extract firmware topologies into the HexisFirmware model.
 */
typedef struct hexis_parser {
    const char* name;
    const char* description;

    /**
     * @brief Fast detection. True if this parser understands the format at the given offset.
     */
    bool (*detect)(const HexisFirmware* fw, uint64_t offset);

    /**
     * @brief Deep parsing. Populates the HexisFirmware partitions/files structures.
     */
    int (*parse)(HexisFirmware* fw, uint64_t offset);

    /**
     * @brief Optional extraction logic (e.g. unpacking SquashFS to disk).
     */
    int (*extract)(const HexisFirmware* fw, uint64_t offset, const char* out_dir);

} hexis_parser_t;

/**
 * @brief Register a parser into the global parser registry.
 */
int hexis_parser_register(const hexis_parser_t* parser);

#ifdef __cplusplus
}
#endif

#endif // HEXIS_PARSER_H
