#ifndef HEXIS_PATCH_H
#define HEXIS_PATCH_H

#include "hexis.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct HexisFirmware;

typedef struct HexisPatch {
    uint32_t id;
    uint64_t offset;
    
    size_t length;
    uint8_t* original_bytes;
    uint8_t* replacement_bytes;
    
    const char* reason;
    uint32_t original_checksum;
    
    struct HexisPatch* next;
} HexisPatch;

typedef struct {
    HexisPatch* head;
    size_t count;
} HexisPatchQueue;

HEXIS_API HexisPatchQueue* hexis_patch_queue_create(void);
HEXIS_API void hexis_patch_queue_free(HexisPatchQueue* q);

HEXIS_API int hexis_patch_add(HexisPatchQueue* q, uint64_t offset, const uint8_t* orig, const uint8_t* repl, size_t len, const char* reason);

/**
 * @brief Apply the patch queue as a transaction.
 * Requires verification. If verification fails, it rolls back.
 */
HEXIS_API int hexis_patch_apply_transaction(struct HexisFirmware* fw, HexisPatchQueue* q);
HEXIS_API int hexis_patch_rollback(struct HexisFirmware* fw, HexisPatchQueue* q);

#ifdef __cplusplus
}
#endif

#endif // HEXIS_PATCH_H
