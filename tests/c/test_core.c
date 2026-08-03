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

#include <stdlib.h>
#include "hexis_scanner.h"
#include "hexis_project.h"
#include "hexis_firmware.h"
#include "hexis_vfs.h"
#include "hexis_event.h"
#include "hexis_job.h"
#include "hexis_patch.h"
#include "hexis_parser.h"

void test_scanner() {
    uint8_t buffer[256] = {0};
    strcpy((char*)buffer + 50, "root:root"); // Inject mock vulnerability
    strcpy((char*)buffer + 100, "nc -e /bin/sh"); // Inject backdoor
    
    HexisGraph* graph = NULL;
    int res = hexis_fw_scan_secrets(buffer, sizeof(buffer), &graph);
    assert(res == 0);
    (void)res;
    assert(graph != NULL);
    assert(graph->node_count == 3);
    
    hexis_graph_free(graph);
    printf("test_scanner passed.\n");
}

void test_project() {
    HexisProject* proj = hexis_project_create("./test_proj", "Test Project");
    assert(proj != NULL);
    int res = hexis_project_save(proj);
    assert(res == 0);
    (void)res;
    hexis_project_close(proj);
    system("rm -rf ./test_proj");
    printf("test_project passed.\n");
}

void test_firmware() {
    HexisFirmware* fw = hexis_firmware_create(1024);
    assert(fw != NULL);
    uint8_t write_buf[] = {0xDE, 0xAD, 0xBE, 0xEF};
    int res = hexis_firmware_write(fw, 128, write_buf, 4);
    assert(res == 4);
    (void)res;
    uint8_t read_buf[4] = {0};
    res = hexis_firmware_read(fw, 128, read_buf, 4);
    assert(res == 4);
    assert(memcmp(read_buf, write_buf, 4) == 0);
    hexis_firmware_free(fw);
    printf("test_firmware passed.\n");
}

static bool event_handled = false;
void mock_event_cb(const hexis_event_t* ev, void* user_data) {
    (void)user_data;
    if (ev->type == HEXIS_EVENT_PROGRAMMER_CONNECTED) {
        event_handled = true;
    }
}

int mock_job_func(HexisJob* job, void* ctx) {
    (void)job; (void)ctx;
    return 0;
}

void test_events_and_jobs() {
    hexis_event_subscribe(HEXIS_EVENT_PROGRAMMER_CONNECTED, mock_event_cb, NULL);
    hexis_event_t ev = {
        .type = HEXIS_EVENT_PROGRAMMER_CONNECTED,
        .payload = NULL,
        .sender_module = "test_core"
    };
    hexis_event_publish(&ev);
    assert(event_handled == true);
    
    HexisJob* job = hexis_job_create("Mock Job", mock_job_func, NULL);
    assert(job != NULL);
    assert(job->status == HEXIS_JOB_PENDING);
    hexis_job_free(job);
    printf("test_events_and_jobs passed.\n");
}

void test_patch_engine() {
    HexisFirmware* fw = hexis_firmware_create(1024);
    HexisPatchQueue* pq = hexis_patch_queue_create();
    uint8_t new_data[] = {0x41, 0x41, 0x41};
    hexis_patch_add(pq, 0x10, NULL, new_data, 3, "Test Patch");
    int res = hexis_patch_apply_transaction(fw, pq);
    assert(res == 0);
    (void)res;
    uint8_t verify[3] = {0};
    hexis_firmware_read(fw, 0x10, verify, 3);
    assert(verify[0] == 0x41);
    hexis_patch_queue_free(pq);
    hexis_firmware_free(fw);
    printf("test_patch_engine passed.\n");
}

static bool mock_parser_detect(const HexisFirmware* fw, uint64_t offset) {
    (void)fw; (void)offset;
    return true;
}

static int mock_parser_parse(HexisFirmware* fw, uint64_t offset) {
    (void)fw; (void)offset;
    return 0;
}

void test_vfs_and_parser() {
    HexisVFS* vfs = hexis_vfs_create();
    assert(vfs != NULL);
    HexisVFSNode* node = hexis_vfs_mount(vfs, "/", 0, 1024, 1);
    assert(node != NULL);
    (void)node;
    hexis_vfs_free(vfs);
    
    hexis_parser_t mock_parser = {
        .name = "Mock Parser",
        .description = "A mock parser for testing",
        .detect = mock_parser_detect,
        .parse = mock_parser_parse,
        .extract = NULL
    };
    int res = hexis_parser_register(&mock_parser);
    assert(res == 0);
    (void)res;
    
    printf("test_vfs_and_parser passed.\n");
}

int main() {
    printf("Running Hexis Core Tests...\n");
    test_chip_lookup();
    test_driver_registration();
    test_scanner();
    test_project();
    test_firmware();
    test_events_and_jobs();
    test_patch_engine();
    test_vfs_and_parser();
    printf("All tests passed successfully!\n");
    return 0;
}
