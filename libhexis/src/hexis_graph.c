#include "hexis_graph.h"
#include <stdlib.h>
#include <string.h>

static uint32_t g_node_id_counter = 1;

HexisGraph* hexis_graph_create(void) {
    HexisGraph* graph = (HexisGraph*)calloc(1, sizeof(HexisGraph));
    if (!graph) return NULL;
    
    graph->capacity = 128;
    graph->nodes = (HexisGraphNode**)calloc(graph->capacity, sizeof(HexisGraphNode*));
    return graph;
}

void hexis_graph_free(HexisGraph* graph) {
    if (!graph) return;
    
    for (size_t i = 0; i < graph->node_count; ++i) {
        HexisGraphNode* node = graph->nodes[i];
        
        HexisGraphEdge* edge = node->edges;
        while (edge) {
            HexisGraphEdge* next = edge->next;
            free(edge);
            edge = next;
        }
        
        if (node->label) free((void*)node->label);
        free(node);
    }
    
    free(graph->nodes);
    free(graph);
}

HexisGraphNode* hexis_graph_add_node(HexisGraph* graph, hexis_node_type_t type, const char* label, void* meta) {
    if (!graph || !label) return NULL;
    
    if (graph->node_count >= graph->capacity) {
        graph->capacity *= 2;
        graph->nodes = (HexisGraphNode**)realloc(graph->nodes, graph->capacity * sizeof(HexisGraphNode*));
    }
    
    HexisGraphNode* node = (HexisGraphNode*)calloc(1, sizeof(HexisGraphNode));
    node->id = g_node_id_counter++;
    node->type = type;
    node->label = strdup(label);
    node->metadata = meta;
    
    graph->nodes[graph->node_count++] = node;
    return node;
}

int hexis_graph_add_edge(HexisGraphNode* source, HexisGraphNode* target, hexis_edge_type_t type) {
    if (!source || !target) return -1;
    
    HexisGraphEdge* edge = (HexisGraphEdge*)calloc(1, sizeof(HexisGraphEdge));
    edge->type = type;
    edge->target = target;
    
    edge->next = source->edges;
    source->edges = edge;
    
    return 0;
}
