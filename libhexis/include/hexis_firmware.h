#ifndef HEXIS_FIRMWARE_H
#define HEXIS_FIRMWARE_H

#include <stdint.h>
#include <stddef.h>
#include "hexis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HEXIS_ARCH_UNKNOWN = 0,
    HEXIS_ARCH_ARM,
    HEXIS_ARCH_ARM64,
    HEXIS_ARCH_MIPS,
    HEXIS_ARCH_MIPS64,
    HEXIS_ARCH_PPC,
    HEXIS_ARCH_X86,
    HEXIS_ARCH_X86_64,
    HEXIS_ARCH_RISCV
} hexis_arch_t;

typedef enum {
    HEXIS_ENDIAN_UNKNOWN = 0,
    HEXIS_ENDIAN_LITTLE,
    HEXIS_ENDIAN_BIG
} hexis_endian_t;

typedef struct {
    uint64_t offset;
    const char* description;
    const char* type; // e.g., "squashfs", "uimage", "gzip"
} hexis_fw_signature_t;

typedef struct {
    double entropy;
    hexis_arch_t architecture;
    hexis_endian_t endianness;
    size_t signature_count;
    hexis_fw_signature_t* signatures;
} hexis_fw_analysis_t;

/**
 * @brief Analyze a firmware buffer in memory.
 * @param buffer Pointer to the firmware data.
 * @param length Size of the firmware data.
 * @param out_analysis Pointer to analysis struct to populate. 
 *                     Must be freed with hexis_fw_analysis_free.
 * @return 0 on success.
 */
HEXIS_API int hexis_fw_analyze(const uint8_t* buffer, size_t length, hexis_fw_analysis_t** out_analysis);

/**
 * @brief Free the analysis structure.
 * @param analysis The analysis structure to free.
 */
HEXIS_API void hexis_fw_analysis_free(hexis_fw_analysis_t* analysis);

/**
 * @brief Calculate the Shannon entropy of a buffer.
 * @param buffer Pointer to data.
 * @param length Size of data.
 * @return Entropy value between 0.0 and 8.0.
 */
HEXIS_API double hexis_fw_calculate_entropy(const uint8_t* buffer, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* HEXIS_FIRMWARE_H */
