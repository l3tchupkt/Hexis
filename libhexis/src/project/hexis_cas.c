#include "hexis_cas.h"
#include "../crypto/sha256.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static char g_cas_dir[1024];

static void hash_to_string(const uint8_t hash[32], char out_str[HEXIS_CAS_HASH_LEN]) {
    for (int i = 0; i < 32; i++) {
        sprintf(&out_str[i * 2], "%02x", hash[i]);
    }
    out_str[64] = '\0';
}

int hexis_cas_init(const char* objects_dir) {
    if (!objects_dir) return -1;
    strncpy(g_cas_dir, objects_dir, sizeof(g_cas_dir) - 1);
    
    // Ensure trailing slash
    if (g_cas_dir[strlen(g_cas_dir) - 1] != '/') {
        strcat(g_cas_dir, "/");
    }
    
    // Create directory (Linux/WSL specific mode for POC)
    mkdir(objects_dir, 0755);
    return 0;
}

int hexis_cas_add(const uint8_t* data, size_t length, hexis_cas_hash_t* out_hash) {
    if (!data || !out_hash) return -1;
    
    hexis_sha256_ctx ctx;
    uint8_t hash[32];
    hexis_sha256_init(&ctx);
    hexis_sha256_update(&ctx, data, length);
    hexis_sha256_final(&ctx, hash);
    
    hash_to_string(hash, out_hash->hash);
    
    char filepath[1200];
    snprintf(filepath, sizeof(filepath), "%s%s", g_cas_dir, out_hash->hash);
    
    // Check if already exists to avoid redundant writes (deduplication)
    struct stat st;
    if (stat(filepath, &st) == 0) {
        return 0; // Already in CAS
    }
    
    FILE* f = fopen(filepath, "wb");
    if (!f) return -2;
    
    fwrite(data, 1, length, f);
    fclose(f);
    
    return 0;
}

int hexis_cas_get_size(const hexis_cas_hash_t* hash, size_t* out_size) {
    if (!hash || !out_size) return -1;
    
    char filepath[1200];
    snprintf(filepath, sizeof(filepath), "%s%s", g_cas_dir, hash->hash);
    
    struct stat st;
    if (stat(filepath, &st) == 0) {
        *out_size = st.st_size;
        return 0;
    }
    return -2;
}

int hexis_cas_get(const hexis_cas_hash_t* hash, uint8_t* out_buffer, size_t max_length) {
    if (!hash || !out_buffer) return -1;
    
    char filepath[1200];
    snprintf(filepath, sizeof(filepath), "%s%s", g_cas_dir, hash->hash);
    
    FILE* f = fopen(filepath, "rb");
    if (!f) return -2;
    
    size_t read_bytes = fread(out_buffer, 1, max_length, f);
    fclose(f);
    
    return (int)read_bytes;
}
