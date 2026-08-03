#include "hexis_vfs.h"
#include "hexis_models.h"
#include <stdlib.h>
#include <string.h>

HexisVFS* hexis_vfs_create(void) {
    HexisVFS* vfs = (HexisVFS*)calloc(1, sizeof(HexisVFS));
    if (!vfs) return NULL;
    
    vfs->root = (HexisVFSNode*)calloc(1, sizeof(HexisVFSNode));
    vfs->root->path = strdup("/");
    vfs->root->is_dir = 1;
    
    return vfs;
}

static void free_node_recursive(HexisVFSNode* node) {
    if (!node) return;
    
    free_node_recursive(node->first_child);
    free_node_recursive(node->next_sibling);
    
    if (node->path) free((void*)node->path);
    free(node);
}

void hexis_vfs_free(HexisVFS* vfs) {
    if (!vfs) return;
    free_node_recursive(vfs->root);
    free(vfs);
}

HexisVFSNode* hexis_vfs_mount(HexisVFS* vfs, const char* mount_point, uint64_t offset, uint64_t size, int is_dir) {
    if (!vfs || !vfs->root || !mount_point) return NULL;
    
    // In a full VFS, this would parse the path and traverse the tree to place the node correctly.
    // For this baseline SDK implementation, we simply attach it to the root.
    
    HexisVFSNode* node = (HexisVFSNode*)calloc(1, sizeof(HexisVFSNode));
    node->path = strdup(mount_point);
    node->is_dir = is_dir;
    node->physical_offset = offset;
    node->size = size;
    
    node->next_sibling = vfs->root->first_child;
    vfs->root->first_child = node;
    
    return node;
}

HexisVFSNode* hexis_vfs_open(HexisVFS* vfs, const char* path) {
    if (!vfs || !vfs->root || !path) return NULL;
    
    HexisVFSNode* curr = vfs->root->first_child;
    while (curr) {
        if (strcmp(curr->path, path) == 0) {
            return curr;
        }
        curr = curr->next_sibling;
    }
    
    return NULL;
}

int hexis_vfs_read(const HexisVFSNode* node, struct HexisFirmware* fw, uint8_t* out_buf, size_t len) {
    if (!node || node->is_dir || !fw || !out_buf) return -1;
    
    size_t to_read = len;
    if (to_read > node->size) {
        to_read = node->size;
    }
    
    return hexis_firmware_read(fw, node->physical_offset, out_buf, to_read);
}
