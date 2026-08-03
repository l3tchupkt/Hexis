#ifndef HEXIS_MODELS_H
#define HEXIS_MODELS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -- Firmware Object Model -- */

typedef struct HexisPartition {
    const char* name;
    uint64_t offset;
    uint64_t size;
    const char* type;
    struct HexisPartition* next;
} HexisPartition;

typedef struct HexisFile {
    const char* path;
    uint64_t offset;
    uint64_t size;
    struct HexisFile* next;
} HexisFile;

typedef struct HexisFirmware {
    uint8_t* buffer;
    size_t size;
    
    // Extracted metadata populated by Parsers
    HexisPartition* partitions;
    HexisFile* files;
    
    // For future memory mapped paging support (opaque)
    void* mmap_handle; 
} HexisFirmware;


/* -- Security Object Model -- */

typedef enum {
    SEVERITY_INFO,
    SEVERITY_LOW,
    SEVERITY_MEDIUM,
    SEVERITY_HIGH,
    SEVERITY_CRITICAL
} HexisSeverity;

typedef struct HexisFinding {
    const char* rule_id;
    const char* title;
    const char* description;
    HexisSeverity severity;
    const char* category;
    
    uint64_t offset;
    char matched_text[256];
    const char* recommendation;
    
    struct HexisFinding* next;
} HexisFinding;

typedef struct HexisReport {
    HexisFirmware* firmware;
    HexisFinding* findings;
    size_t finding_count;
    double risk_score;
} HexisReport;


/* -- Security Rule Engine Model -- */

typedef struct HexisSecurityRule {
    const char* id;
    const char* title;
    const char* description;
    HexisSeverity severity;
    const char* category;
    const char* recommendation;
    
    /**
     * @brief Matcher function
     * @return True if rule matches at the given buffer location, populating out_finding.
     */
    bool (*match)(const uint8_t* buffer, size_t length, uint64_t offset, struct HexisFinding* out_finding);
} HexisSecurityRule;


/* API */

HexisFirmware* hexis_firmware_create(size_t size);
void hexis_firmware_free(HexisFirmware* fw);
int hexis_firmware_read(const HexisFirmware* fw, uint64_t offset, uint8_t* out_buf, size_t len);
int hexis_firmware_write(HexisFirmware* fw, uint64_t offset, const uint8_t* in_buf, size_t len);

HexisReport* hexis_report_create(HexisFirmware* fw);
void hexis_report_free(HexisReport* report);
void hexis_report_add_finding(HexisReport* report, HexisFinding* finding);

#ifdef __cplusplus
}
#endif

#endif // HEXIS_MODELS_H
