#include "../include/hexis_firmware.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

double hexis_fw_calculate_entropy(const uint8_t* buffer, size_t length) {
    if (!buffer || length == 0) return 0.0;
    
    size_t counts[256] = {0};
    for (size_t i = 0; i < length; ++i) {
        counts[buffer[i]]++;
    }
    
    double entropy = 0.0;
    for (int i = 0; i < 256; ++i) {
        if (counts[i] > 0) {
            double p = (double)counts[i] / (double)length;
            entropy -= p * log2(p);
        }
    }
    return entropy;
}

// ELF Header magic: 7F 45 4C 46
static void analyze_elf(const uint8_t* buffer, size_t length, hexis_fw_analysis_t* analysis) {
    if (length < 64) return; // Too small for ELF header
    
    if (buffer[0] == 0x7F && buffer[1] == 'E' && buffer[2] == 'L' && buffer[3] == 'F') {
        // EI_DATA at offset 5
        if (buffer[5] == 1) {
            analysis->endianness = HEXIS_ENDIAN_LITTLE;
        } else if (buffer[5] == 2) {
            analysis->endianness = HEXIS_ENDIAN_BIG;
        }
        
        // e_machine at offset 18 (assuming 64-bit or 32-bit, the offset is the same for e_machine)
        uint16_t machine = 0;
        if (analysis->endianness == HEXIS_ENDIAN_LITTLE) {
            machine = (buffer[19] << 8) | buffer[18];
        } else {
            machine = (buffer[18] << 8) | buffer[19];
        }
        
        switch (machine) {
            case 3: analysis->architecture = HEXIS_ARCH_X86; break;
            case 8: analysis->architecture = HEXIS_ARCH_MIPS; break;
            case 20: analysis->architecture = HEXIS_ARCH_PPC; break;
            case 40: analysis->architecture = HEXIS_ARCH_ARM; break;
            case 62: analysis->architecture = HEXIS_ARCH_X86_64; break;
            case 183: analysis->architecture = HEXIS_ARCH_ARM64; break;
            case 243: analysis->architecture = HEXIS_ARCH_RISCV; break;
        }
    }
}

// Very basic signature scanner for Phase 7
static void scan_signatures(const uint8_t* buffer, size_t length, hexis_fw_analysis_t* analysis) {
    // We will dynamically allocate signatures as we find them.
    // For a real pentesting framework, this uses a robust state machine or Aho-Corasick.
    size_t capacity = 10;
    analysis->signatures = (hexis_fw_signature_t*)malloc(capacity * sizeof(hexis_fw_signature_t));
    analysis->signature_count = 0;

    for (size_t i = 0; i < length - 4; ++i) {
        if (analysis->signature_count >= capacity) break; // Keep it simple for now
        
        // SquashFS (hsqs / sqsh)
        if ((buffer[i] == 'h' && buffer[i+1] == 's' && buffer[i+2] == 'q' && buffer[i+3] == 's') ||
            (buffer[i] == 's' && buffer[i+1] == 'q' && buffer[i+2] == 's' && buffer[i+3] == 'h')) {
            analysis->signatures[analysis->signature_count].offset = i;
            analysis->signatures[analysis->signature_count].type = "squashfs";
            analysis->signatures[analysis->signature_count].description = "SquashFS filesystem";
            analysis->signature_count++;
        }
        
        // gzip (1F 8B 08)
        else if (buffer[i] == 0x1F && buffer[i+1] == 0x8B && buffer[i+2] == 0x08) {
            analysis->signatures[analysis->signature_count].offset = i;
            analysis->signatures[analysis->signature_count].type = "gzip";
            analysis->signatures[analysis->signature_count].description = "gzip compressed data";
            analysis->signature_count++;
        }
        
        // uImage (27 05 19 56)
        else if (buffer[i] == 0x27 && buffer[i+1] == 0x05 && buffer[i+2] == 0x19 && buffer[i+3] == 0x56) {
            analysis->signatures[analysis->signature_count].offset = i;
            analysis->signatures[analysis->signature_count].type = "uimage";
            analysis->signatures[analysis->signature_count].description = "U-Boot uImage firmware image";
            analysis->signature_count++;
        }
    }
}

int hexis_fw_analyze(const uint8_t* buffer, size_t length, hexis_fw_analysis_t** out_analysis) {
    if (!buffer || length == 0 || !out_analysis) return -1;
    
    hexis_fw_analysis_t* analysis = (hexis_fw_analysis_t*)malloc(sizeof(hexis_fw_analysis_t));
    if (!analysis) return -2;
    
    analysis->architecture = HEXIS_ARCH_UNKNOWN;
    analysis->endianness = HEXIS_ENDIAN_UNKNOWN;
    analysis->signature_count = 0;
    analysis->signatures = nullptr;
    
    // 1. Calculate Entropy
    analysis->entropy = hexis_fw_calculate_entropy(buffer, length);
    
    // 2. ELF Header Analysis (if the file itself is an ELF, common in unpacked firmwares or baremetal)
    analyze_elf(buffer, length, analysis);
    
    // 3. Scan for nested filesystems/compressions
    scan_signatures(buffer, length, analysis);
    
    *out_analysis = analysis;
    return 0;
}

void hexis_fw_analysis_free(hexis_fw_analysis_t* analysis) {
    if (!analysis) return;
    if (analysis->signatures) {
        free(analysis->signatures);
    }
    free(analysis);
}
