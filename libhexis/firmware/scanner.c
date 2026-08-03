#include "hexis_scanner.h"
#include "hexis_models.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Proof of Concept Native Rule Engine */

static bool rule_default_creds_match(const uint8_t* buffer, size_t length, uint64_t offset, HexisFinding* out) {
    if (offset + 9 > length) return false;
    if (memcmp(buffer + offset, "root:root", 9) == 0) {
        snprintf(out->matched_text, sizeof(out->matched_text), "root:root");
        return true;
    }
    return false;
}

static bool rule_netcat_match(const uint8_t* buffer, size_t length, uint64_t offset, HexisFinding* out) {
    if (offset + 13 > length) return false;
    if (memcmp(buffer + offset, "nc -e /bin/sh", 13) == 0) {
        snprintf(out->matched_text, sizeof(out->matched_text), "nc -e /bin/sh");
        return true;
    }
    return false;
}

static HexisSecurityRule g_builtin_rules[] = {
    {
        .id = "VULN-001",
        .title = "Default Root Credentials",
        .description = "Found hardcoded 'root:root' shadow hash / credentials",
        .severity = SEVERITY_CRITICAL,
        .category = "Authentication",
        .recommendation = "Remove hardcoded credentials and enforce unique passwords.",
        .match = rule_default_creds_match
    },
    {
        .id = "VULN-002",
        .title = "Netcat Backdoor Shell",
        .description = "Found execution of netcat binding a shell",
        .severity = SEVERITY_HIGH,
        .category = "Backdoor",
        .recommendation = "Remove backdoor debugging tools from production builds.",
        .match = rule_netcat_match
    }
};

static const size_t g_num_rules = sizeof(g_builtin_rules) / sizeof(g_builtin_rules[0]);

#include "hexis_graph.h"

int hexis_fw_scan_secrets(const uint8_t* buffer, size_t length, HexisGraph** out_graph) {
    if (!buffer || !out_graph) return -1;
    
    *out_graph = hexis_graph_create();
    if (!*out_graph) return -1;
    
    HexisGraphNode* root_node = hexis_graph_add_node(*out_graph, NODE_FIRMWARE, "Scanned Firmware Buffer", NULL);
    
    for (size_t i = 0; i < length; ++i) {
        for (size_t r = 0; r < g_num_rules; ++r) {
            HexisFinding* generic_finding = (HexisFinding*)calloc(1, sizeof(HexisFinding));
            
            if (g_builtin_rules[r].match(buffer, length, i, generic_finding)) {
                generic_finding->offset = i;
                generic_finding->description = g_builtin_rules[r].title;
                
                HexisGraphNode* finding_node = hexis_graph_add_node(*out_graph, NODE_FINDING, g_builtin_rules[r].id, generic_finding);
                hexis_graph_add_edge(root_node, finding_node, EDGE_CONTAINS);
            } else {
                free(generic_finding);
            }
        }
    }
    
    return 0;
}

// Legacy scan API removed in favor of hexis_graph_free
