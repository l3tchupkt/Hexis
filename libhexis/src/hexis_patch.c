#include "hexis_patch.h"
#include "hexis_models.h" // For HexisFirmware definition and read/write
#include <stdlib.h>
#include <string.h>

static uint32_t g_patch_id = 1;

HexisPatchQueue* hexis_patch_queue_create(void) {
    return (HexisPatchQueue*)calloc(1, sizeof(HexisPatchQueue));
}

void hexis_patch_queue_free(HexisPatchQueue* q) {
    if (!q) return;
    
    HexisPatch* p = q->head;
    while (p) {
        HexisPatch* next = p->next;
        if (p->original_bytes) free(p->original_bytes);
        if (p->replacement_bytes) free(p->replacement_bytes);
        if (p->reason) free((void*)p->reason);
        free(p);
        p = next;
    }
    free(q);
}

int hexis_patch_add(HexisPatchQueue* q, uint64_t offset, const uint8_t* orig, const uint8_t* repl, size_t len, const char* reason) {
    if (!q || !repl || len == 0) return -1;
    
    HexisPatch* p = (HexisPatch*)calloc(1, sizeof(HexisPatch));
    p->id = g_patch_id++;
    p->offset = offset;
    p->length = len;
    
    if (orig) {
        p->original_bytes = (uint8_t*)malloc(len);
        memcpy(p->original_bytes, orig, len);
    }
    
    p->replacement_bytes = (uint8_t*)malloc(len);
    memcpy(p->replacement_bytes, repl, len);
    
    if (reason) {
        p->reason = strdup(reason);
    }
    
    p->next = q->head;
    q->head = p;
    q->count++;
    
    return 0;
}

int hexis_patch_apply_transaction(struct HexisFirmware* fw, HexisPatchQueue* q) {
    if (!fw || !q) return -1;
    
    // In a real hardware transaction, this would call hexis_driver_t->write and hexis_driver_t->verify.
    // Since HexisFirmware wraps the memory buffer, we apply the patches directly to the buffer here.
    
    HexisPatch* p = q->head;
    while (p) {
        hexis_firmware_write(fw, p->offset, p->replacement_bytes, p->length);
        p = p->next;
    }
    
    return 0; // Success
}

int hexis_patch_rollback(struct HexisFirmware* fw, HexisPatchQueue* q) {
    if (!fw || !q) return -1;
    
    HexisPatch* p = q->head;
    while (p) {
        if (p->original_bytes) {
            hexis_firmware_write(fw, p->offset, p->original_bytes, p->length);
        }
        p = p->next;
    }
    return 0;
}
