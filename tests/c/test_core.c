#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "hexis.h"
#include "hexis_chip.h"
#include "hexis_driver.h"

void test_chip_lookup() {
    const hexis_chip_info_t* chip = hexis_chip_lookup_by_jedec(0xEF4017);
    assert(chip != nullptr);
    assert(strcmp(chip->manufacturer, "Winbond") == 0);
    assert(chip->capacity_bytes == 8 * 1024 * 1024);
    (void)chip;
    
    const hexis_chip_info_t* chip2 = hexis_chip_lookup_by_name("Macronix", "MX25L12835F");
    assert(chip2 != nullptr);
    assert(chip2->jedec_id == 0xC22018);
    (void)chip2;
    
    assert(hexis_chip_lookup_by_jedec(0xDEADBEEF) == nullptr);
    printf("test_chip_lookup passed.\n");
}

int mock_connect(hexis_device_context_t** ctx) {
    (void)ctx;
    return 0;
}

void test_driver_registration() {
    hexis_driver_t mock_driver = {
        .name = "mock",
        .description = "A mock driver for testing",
        .connect = mock_connect
    };
    
    int res = hexis_driver_register(&mock_driver);
    assert(res == 0);
    
    // Duplicate registration should fail
    res = hexis_driver_register(&mock_driver);
    assert(res != 0);
    (void)res;
    
    const hexis_driver_t* fetched = hexis_driver_get("mock");
    assert(fetched != nullptr);
    assert(strcmp(fetched->name, "mock") == 0);
    (void)fetched;
    
    printf("test_driver_registration passed.\n");
}

int main() {
    printf("Running Hexis Core Tests...\n");
    test_chip_lookup();
    test_driver_registration();
    printf("All tests passed successfully!\n");
    return 0;
}
