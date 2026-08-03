#include "../include/hexis_scanner.h"
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdio.h>

typedef struct {
    hexis_finding_type_t type;
    const char* pattern;
    const char* description;
} secret_regex_t;

// Some basic patterns for Phase 8
static const secret_regex_t patterns[] = {
    { HEXIS_SECRET_AWS_KEY, "AKIA[0-9A-Z]{16}", "AWS Access Key ID" },
    { HEXIS_SECRET_PRIVATE_KEY, "-----BEGIN [A-Z ]*PRIVATE KEY-----", "Private Cryptographic Key" },
    { HEXIS_SECRET_JWT, "eyJ[A-Za-z0-9-_=]+\\.[A-Za-z0-9-_=]+\\.?[A-Za-z0-9-_.+/=]*", "JSON Web Token (JWT)" },
    { HEXIS_VULN_WEAK_CRYPTO, "DES-CBC", "Weak Cryptography Algorithm (DES)" },
    { HEXIS_VULN_DEFAULT_CREDS, "root:root", "Default Root Credentials" }
};

static const size_t num_patterns = sizeof(patterns) / sizeof(patterns[0]);

int hexis_fw_scan_secrets(const uint8_t* buffer, size_t length, hexis_scan_report_t** out_report) {
    if (!buffer || length == 0 || !out_report) return -1;
    
    // For large firmware blobs, doing regex on binary data is tricky and slow.
    // In a real scenario, we'd only scan printable strings or extract files first.
    // Here we'll do a naive search on the raw buffer up to a certain limit or treat it as a string
    // if we add a null terminator, but binary data contains nulls.
    // For Phase 8 demonstration, we will do basic block-based text scanning.
    
    hexis_scan_report_t* report = (hexis_scan_report_t*)malloc(sizeof(hexis_scan_report_t));
    if (!report) return -2;
    
    size_t capacity = 10;
    report->count = 0;
    report->findings = (hexis_finding_t*)malloc(capacity * sizeof(hexis_finding_t));
    
    // Convert a chunk to a C-string for easy regex (mocking the extraction phase)
    // We limit to 1MB for this naive implementation
    size_t scan_len = length > 1024 * 1024 ? 1024 * 1024 : length;
    char* text_buffer = (char*)malloc(scan_len + 1);
    if (!text_buffer) {
        free(report->findings);
        free(report);
        return -2;
    }
    
    // Replace null bytes with spaces so regex doesn't terminate early
    for(size_t i=0; i<scan_len; i++) {
        text_buffer[i] = (buffer[i] == 0) ? ' ' : buffer[i];
    }
    text_buffer[scan_len] = '\0';
    
    for (size_t p = 0; p < num_patterns; ++p) {
        regex_t regex;
        if (regcomp(&regex, patterns[p].pattern, REG_EXTENDED) != 0) {
            continue;
        }
        
        regmatch_t match;
        const char* ptr = text_buffer;
        size_t offset = 0;
        
        while (regexec(&regex, ptr, 1, &match, 0) == 0) {
            if (report->count >= capacity) {
                // Keep it simple for phase 8, stop allocating
                break;
            }
            
            report->findings[report->count].type = patterns[p].type;
            report->findings[report->count].description = patterns[p].description;
            report->findings[report->count].offset = offset + match.rm_so;
            
            size_t match_len = match.rm_eo - match.rm_so;
            if (match_len > 127) match_len = 127;
            strncpy(report->findings[report->count].matched_text, ptr + match.rm_so, match_len);
            report->findings[report->count].matched_text[match_len] = '\0';
            
            report->count++;
            
            ptr += match.rm_eo;
            offset += match.rm_eo;
        }
        regfree(&regex);
    }
    
    free(text_buffer);
    *out_report = report;
    return 0;
}

void hexis_scan_report_free(hexis_scan_report_t* report) {
    if (!report) return;
    if (report->findings) {
        free(report->findings);
    }
    free(report);
}
