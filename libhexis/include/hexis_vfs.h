#ifndef HEXIS_VFS_H
#define HEXIS_VFS_H

#include "hexis.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct HexisFirmware;

typedef struct HexisVFSNode {
    const char* path;
    int is_dir;
    uint64_t physical_offset;
    uint64_t size;
    struct HexisVFSNode* first_child;
    struct HexisVFSNode* next_sibling;
} HexisVFSNode;

typedef struct {
    HexisVFSNode* root;
} HexisVFS;

HEXIS_API HexisVFS* hexis_vfs_create(void);
HEXIS_API void hexis_vfs_free(HexisVFS* vfs);

HEXIS_API HexisVFSNode* hexis_vfs_mount(HexisVFS* vfs, const char* mount_point, uint64_t offset, uint64_t size, int is_dir);
HEXIS_API HexisVFSNode* hexis_vfs_open(HexisVFS* vfs, const char* path);
HEXIS_API int hexis_vfs_read(const HexisVFSNode* node, struct HexisFirmware* fw, uint8_t* out_buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif // HEXIS_VFS_H
