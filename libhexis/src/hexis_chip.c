#include "hexis_chip.h"
#include <string.h>

// A small internal mock database of chips for Phase 2
static const hexis_chip_info_t internal_chip_db[] = {
    {
        .manufacturer = "Winbond",
        .model = "W25Q64JV",
        .capacity_bytes = 8 * 1024 * 1024,
        .sector_size = 4096,
        .page_size = 256,
        .voltage = 3.3f,
        .jedec_id = 0xEF4017,
        .supported_operations = HEXIS_OP_READ | HEXIS_OP_WRITE | HEXIS_OP_ERASE | HEXIS_OP_VERIFY | HEXIS_OP_STATUS,
        .memory_type = HEXIS_MEM_TYPE_SPI_NOR
    },
    {
        .manufacturer = "Macronix",
        .model = "MX25L12835F",
        .capacity_bytes = 16 * 1024 * 1024,
        .sector_size = 4096,
        .page_size = 256,
        .voltage = 3.3f,
        .jedec_id = 0xC22018,
        .supported_operations = HEXIS_OP_READ | HEXIS_OP_WRITE | HEXIS_OP_ERASE | HEXIS_OP_VERIFY | HEXIS_OP_STATUS,
        .memory_type = HEXIS_MEM_TYPE_SPI_NOR
    }
};

static const size_t internal_chip_db_size = sizeof(internal_chip_db) / sizeof(internal_chip_db[0]);

const hexis_chip_info_t* hexis_chip_lookup_by_jedec(uint32_t jedec_id) {
    for (size_t i = 0; i < internal_chip_db_size; ++i) {
        if (internal_chip_db[i].jedec_id == jedec_id) {
            return &internal_chip_db[i];
        }
    }
    return nullptr; // In C23, nullptr is standard
}

const hexis_chip_info_t* hexis_chip_lookup_by_name(const char* manufacturer, const char* model) {
    if (!manufacturer || !model) return nullptr;
    
    for (size_t i = 0; i < internal_chip_db_size; ++i) {
        if (strcmp(internal_chip_db[i].manufacturer, manufacturer) == 0 &&
            strcmp(internal_chip_db[i].model, model) == 0) {
            return &internal_chip_db[i];
        }
    }
    return nullptr;
}
