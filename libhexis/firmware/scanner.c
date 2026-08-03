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

int hexis_fw_scan_secrets(const uint8_t* buffer, size_t length, hexis_scan_report_t** out_report) {
    if (!buffer || !out_report) return -1;
    
    // Legacy support implementation bridging to new engine logic
    *out_report = calloc(1, sizeof(hexis_scan_report_t));
    if (!*out_report) return -1;
    
    // Allocate space for up to 128 findings for this POC
    size_t capacity = 128;
    (*out_report)->findings = calloc(capacity, sizeof(hexis_finding_t));
    
    for (size_t i = 0; i < length; ++i) {
        for (size_t r = 0; r < g_num_rules; ++r) {
            HexisFinding generic_finding = {0};
            if (g_builtin_rules[r].match(buffer, length, i, &generic_finding)) {
                
                if ((*out_report)->count < capacity) {
                    hexis_finding_t* f = &(*out_report)->findings[(*out_report)->count++];
                    f->offset = i;
                    f->description = g_builtin_rules[r].title;
                    f->type = HEXIS_VULN_DEFAULT_CREDS; // Map to legacy enum temporarily
                    strncpy(f->matched_text, generic_finding.matched_text, sizeof(f->matched_text) - 1);
                }
            }
        }
    }
    
    return 0;
}

void hexis_scan_report_free(hexis_scan_report_t* report) {
    if (!report) return;
    if (report->findings) free(report->findings);
    free(report);
}
