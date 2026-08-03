#ifndef HEXIS_DRIVER_H
#define HEXIS_DRIVER_H

#include <stdint.h>
#include <stddef.h>

#include "hexis.h"
#include "hexis_chip.h"

#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration of the internal device handle context
typedef struct hexis_device_context hexis_device_context_t;

typedef enum {
    CAP_SPI       = (1 << 0),
    CAP_UART      = (1 << 1),
    CAP_GPIO      = (1 << 2),
    CAP_I2C       = (1 << 3),
    CAP_EEPROM    = (1 << 4),
    CAP_NOR       = (1 << 5),
    CAP_NAND      = (1 << 6),
    CAP_OTP       = (1 << 7),
    CAP_QUAD_SPI  = (1 << 8)
} hexis_capability_t;

/**
 * @brief Driver operations table representing a hardware programmer interface.
 */
typedef struct {
    const char* name;
    const char* description;
    uint32_t capabilities; // Bitmask of hexis_capability_t
    
    // Connect to the hardware programmer
    int (*connect)(hexis_device_context_t** ctx);
    
    // Disconnect from the hardware programmer
    void (*disconnect)(hexis_device_context_t* ctx);
    
    // Probe the bus to find connected chips
    int (*probe)(hexis_device_context_t* ctx, hexis_chip_info_t* detected_chip);
    
    // Read data from the flash memory
    int (*read)(hexis_device_context_t* ctx, uint64_t address, uint8_t* buffer, size_t length);
    
    // Write data to the flash memory
    int (*write)(hexis_device_context_t* ctx, uint64_t address, const uint8_t* buffer, size_t length);
    
    // Erase a specific region or the entire chip
    int (*erase)(hexis_device_context_t* ctx, uint64_t address, size_t length);
    
    // Verify flash contents against a buffer
    int (*verify)(hexis_device_context_t* ctx, uint64_t address, const uint8_t* buffer, size_t length);
    
    // Read/Write status register(s)
    int (*status)(hexis_device_context_t* ctx, uint8_t* status_reg, bool write);
    
    // Identify the connected programmer hardware (e.g., firmware version, serial)
    int (*identify)(hexis_device_context_t* ctx, char* identity_buf, size_t buf_len);

    // Transaction System
    int (*transaction_begin)(hexis_device_context_t* ctx);
    int (*transaction_commit)(hexis_device_context_t* ctx);
    int (*transaction_rollback)(hexis_device_context_t* ctx);

} hexis_driver_t;

/**
 * @brief Register a new hardware driver into the Hexis framework.
 * @param driver The driver interface implementation.
 * @return 0 on success, non-zero on failure.
 */
HEXIS_API int hexis_driver_register(const hexis_driver_t* driver);

/**
 * @brief Get a registered driver by its name.
 * @param name The driver name.
 * @return Pointer to the driver, or nullptr if not found.
 */
HEXIS_API const hexis_driver_t* hexis_driver_get(const char* name);

#ifdef __cplusplus
}
#endif

#endif /* HEXIS_DRIVER_H */
