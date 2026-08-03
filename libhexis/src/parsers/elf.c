#include "hexis_parser.h"
#include <string.h>

static bool elf_detect(const HexisFirmware* fw, uint64_t offset) {
    if (!fw || offset + 4 > fw->size) return false;
    
    // Check for standard ELF magic: 0x7F 'E' 'L' 'F'
    if (fw->buffer[offset] == 0x7F && 
        fw->buffer[offset+1] == 'E' && 
        fw->buffer[offset+2] == 'L' && 
        fw->buffer[offset+3] == 'F') {
        return true;
    }
    return false;
}

static int elf_parse(HexisFirmware* fw, uint64_t offset) {
    (void)offset;
    if (!fw) return -1;
    
    // This is a proof-of-concept plugin demonstrating architecture.
    // A full ELF parser would traverse the program headers and sections here.
    
    return 0;
}

static int elf_extract(const HexisFirmware* fw, uint64_t offset, const char* out_dir) {
    (void)fw;
    (void)offset;
    (void)out_dir;
    return -1; // Extraction not applicable for bare ELF unless it's a bundle
}

static hexis_parser_t elf_parser = {
    .name = "ELF Parser",
    .description = "Native Executable and Linkable Format (ELF) parser",
    .detect = elf_detect,
    .parse = elf_parse,
    .extract = elf_extract
};

// Auto-register via GCC constructor attribute so it hooks natively
__attribute__((constructor)) static void register_elf_parser(void) {
    hexis_parser_register(&elf_parser);
}
