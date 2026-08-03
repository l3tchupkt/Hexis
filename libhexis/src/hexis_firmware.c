#include "hexis_models.h"
#include <stdlib.h>
#include <string.h>

HexisFirmware* hexis_firmware_create(size_t size) {
    HexisFirmware* fw = (HexisFirmware*)calloc(1, sizeof(HexisFirmware));
    if (!fw) return NULL;
    
    fw->size = size;
    fw->buffer = (uint8_t*)calloc(1, size);
    if (!fw->buffer) {
        free(fw);
        return NULL;
    }
    
    return fw;
}

void hexis_firmware_free(HexisFirmware* fw) {
    if (!fw) return;
    
    if (fw->buffer) {
        free(fw->buffer);
    }
    
    // Free partitions
    HexisPartition* p = fw->partitions;
    while (p) {
        HexisPartition* next = p->next;
        free(p);
        p = next;
    }
    
    // Free files
    HexisFile* f = fw->files;
    while (f) {
        HexisFile* next = f->next;
        free(f);
        f = next;
    }
    
    free(fw);
}

int hexis_firmware_read(const HexisFirmware* fw, uint64_t offset, uint8_t* out_buf, size_t len) {
    if (!fw || !out_buf) return -1;
    if (offset >= fw->size) return -1;
    
    size_t to_read = len;
    if (offset + to_read > fw->size) {
        to_read = fw->size - offset;
    }
    
    memcpy(out_buf, fw->buffer + offset, to_read);
    return (int)to_read;
}

int hexis_firmware_write(HexisFirmware* fw, uint64_t offset, const uint8_t* in_buf, size_t len) {
    if (!fw || !in_buf) return -1;
    if (offset >= fw->size) return -1;
    
    size_t to_write = len;
    if (offset + to_write > fw->size) {
        to_write = fw->size - offset;
    }
    
    memcpy(fw->buffer + offset, in_buf, to_write);
    return (int)to_write;
}
