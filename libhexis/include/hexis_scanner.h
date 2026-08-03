#ifndef HEXIS_SCANNER_H
#define HEXIS_SCANNER_H

#include <stdint.h>
#include <stddef.h>
#include "hexis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HEXIS_SECRET_AWS_KEY,
    HEXIS_SECRET_PRIVATE_KEY,
    HEXIS_SECRET_SSH_KEY,
    HEXIS_SECRET_JWT,
    HEXIS_SECRET_API_TOKEN,
    HEXIS_VULN_WEAK_CRYPTO,
    HEXIS_VULN_DEFAULT_CREDS
} hexis_finding_type_t;

typedef struct {
    hexis_finding_type_t type;
    uint64_t offset;
    const char* description;
    char matched_text[128]; // Snippet of the finding
} hexis_finding_t;

typedef struct {
    size_t count;
    hexis_finding_t* findings;
} hexis_scan_report_t;

/**
 * @brief Scan a firmware buffer for secrets and vulnerabilities.
 * @param buffer Pointer to the firmware data.
 * @param length Size of the firmware data.
 * @param out_report Pointer to report struct to populate. 
 *                   Must be freed with hexis_scan_report_free.
 * @return 0 on success.
 */
HEXIS_API int hexis_fw_scan_secrets(const uint8_t* buffer, size_t length, hexis_scan_report_t** out_report);

/**
 * @brief Free the scan report.
 * @param report The report to free.
 */
HEXIS_API void hexis_scan_report_free(hexis_scan_report_t* report);

#ifdef __cplusplus
}
#endif

#endif /* HEXIS_SCANNER_H */
