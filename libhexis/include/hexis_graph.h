#ifndef HEXIS_GRAPH_H
#define HEXIS_GRAPH_H

#include "hexis.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NODE_FIRMWARE,
    NODE_PARTITION,
    NODE_FILESYSTEM,
    NODE_EXECUTABLE,
    NODE_LIBRARY,
    NODE_FUNCTION,
    NODE_FINDING,
    NODE_CVE,
    NODE_SECRET
} hexis_node_type_t;

typedef struct HexisGraphNode {
    uint32_t id;
    hexis_node_type_t type;
    const char* label;
    void* metadata; // Pointer to specific HexisFinding, HexisFile, etc.
    
    struct HexisGraphEdge* edges;
} HexisGraphNode;

typedef enum {
    EDGE_CONTAINS,
    EDGE_EXHIBITS,
    EDGE_DEPENDS_ON,
    EDGE_REFERENCES
} hexis_edge_type_t;

typedef struct HexisGraphEdge {
    hexis_edge_type_t type;
    HexisGraphNode* target;
    struct HexisGraphEdge* next;
} HexisGraphEdge;

typedef struct {
    HexisGraphNode** nodes;
    size_t node_count;
    size_t capacity;
} HexisGraph;

HEXIS_API HexisGraph* hexis_graph_create(void);
HEXIS_API void hexis_graph_free(HexisGraph* graph);
HEXIS_API HexisGraphNode* hexis_graph_add_node(HexisGraph* graph, hexis_node_type_t type, const char* label, void* meta);
HEXIS_API int hexis_graph_add_edge(HexisGraphNode* source, HexisGraphNode* target, hexis_edge_type_t type);

#ifdef __cplusplus
}
#endif

#endif // HEXIS_GRAPH_H
