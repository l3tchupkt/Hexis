#ifndef HEXIS_SHA256_H
#define HEXIS_SHA256_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t data[64];
    uint32_t datalen;
    uint64_t bitlen;
    uint32_t state[8];
} hexis_sha256_ctx;

void hexis_sha256_init(hexis_sha256_ctx *ctx);
void hexis_sha256_update(hexis_sha256_ctx *ctx, const uint8_t data[], size_t len);
void hexis_sha256_final(hexis_sha256_ctx *ctx, uint8_t hash[32]);

#ifdef __cplusplus
}
#endif

#endif // HEXIS_SHA256_H
