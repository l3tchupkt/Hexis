#ifndef HEXIS_CHIP_H
#define HEXIS_CHIP_H

#include <stdint.h>
#include <stddef.h>

#include "hexis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HEXIS_MEM_TYPE_UNKNOWN = 0,
    HEXIS_MEM_TYPE_SPI_NOR,
    HEXIS_MEM_TYPE_SPI_NAND,
    HEXIS_MEM_TYPE_I2C_EEPROM,
    HEXIS_MEM_TYPE_PARALLEL_NOR,
    HEXIS_MEM_TYPE_PARALLEL_NAND
} hexis_mem_type_t;

typedef enum {
    HEXIS_OP_NONE   = 0,
    HEXIS_OP_READ   = 1 << 0,
    HEXIS_OP_WRITE  = 1 << 1,
    HEXIS_OP_ERASE  = 1 << 2,
    HEXIS_OP_VERIFY = 1 << 3,
    HEXIS_OP_OTP    = 1 << 4,
    HEXIS_OP_STATUS = 1 << 5
} hexis_supported_op_t;

typedef struct {
    const char* manufacturer;
    const char* model;
    uint64_t capacity_bytes;
    uint32_t sector_size;
    uint32_t page_size;
    float voltage;
    uint32_t jedec_id;
    uint32_t supported_operations; // Bitmask of hexis_supported_op_t
    hexis_mem_type_t memory_type;
} hexis_chip_info_t;

/**
 * @brief Find a chip by its JEDEC ID.
 * @param jedec_id The JEDEC ID to look up.
 * @return Pointer to chip info, or nullptr if not found.
 */
HEXIS_API const hexis_chip_info_t* hexis_chip_lookup_by_jedec(uint32_t jedec_id);

/**
 * @brief Find a chip by its manufacturer and model strings.
 * @param manufacturer The manufacturer name.
 * @param model The model name.
 * @return Pointer to chip info, or nullptr if not found.
 */
HEXIS_API const hexis_chip_info_t* hexis_chip_lookup_by_name(const char* manufacturer, const char* model);

#ifdef __cplusplus
}
#endif

#endif /* HEXIS_CHIP_H */
