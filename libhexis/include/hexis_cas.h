#ifndef HEXIS_CAS_H
#define HEXIS_CAS_H

#include "hexis.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// SHA-256 hash string representation (64 chars + null terminator)
#define HEXIS_CAS_HASH_LEN 65

typedef struct {
    char hash[HEXIS_CAS_HASH_LEN];
} hexis_cas_hash_t;

/**
 * @brief Initialize the CAS storage directory.
 * @param objects_dir Path to the CAS objects directory.
 * @return 0 on success.
 */
HEXIS_API int hexis_cas_init(const char* objects_dir);

/**
 * @brief Add a data blob to the CAS.
 * @param data The binary data to store.
 * @param length The size of the data.
 * @param out_hash The computed SHA-256 hash string (output).
 * @return 0 on success.
 */
HEXIS_API int hexis_cas_add(const uint8_t* data, size_t length, hexis_cas_hash_t* out_hash);

/**
 * @brief Retrieve a blob's size from the CAS.
 */
HEXIS_API int hexis_cas_get_size(const hexis_cas_hash_t* hash, size_t* out_size);

/**
 * @brief Retrieve a blob from the CAS.
 */
HEXIS_API int hexis_cas_get(const hexis_cas_hash_t* hash, uint8_t* out_buffer, size_t max_length);

#ifdef __cplusplus
}
#endif

#endif // HEXIS_CAS_H
