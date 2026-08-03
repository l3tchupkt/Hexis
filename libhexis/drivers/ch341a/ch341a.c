#include "ch341a.h"
#include "../../include/hexis_driver.h"
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CH341A_VID 0x1A86
#define CH341A_PID 0x5512

struct hexis_device_context {
    libusb_context *usb_ctx;
    libusb_device_handle *dev_handle;
};

static int ch341a_connect(hexis_device_context_t** out_ctx) {
    hexis_device_context_t* ctx = (hexis_device_context_t*)malloc(sizeof(hexis_device_context_t));
    if (!ctx) return -1;
    
    if (libusb_init(&ctx->usb_ctx) < 0) {
        free(ctx);
        return -2;
    }
    
    ctx->dev_handle = libusb_open_device_with_vid_pid(ctx->usb_ctx, CH341A_VID, CH341A_PID);
    if (!ctx->dev_handle) {
        libusb_exit(ctx->usb_ctx);
        free(ctx);
        return -3; // Device not found
    }
    
    if (libusb_claim_interface(ctx->dev_handle, 0) < 0) {
        libusb_close(ctx->dev_handle);
        libusb_exit(ctx->usb_ctx);
        free(ctx);
        return -4;
    }
    
    // Additional specific CH341A SPI init sequence goes here (e.g. setting SPI speed)
    *out_ctx = ctx;
    return 0;
}

static void ch341a_disconnect(hexis_device_context_t* ctx) {
    if (!ctx) return;
    if (ctx->dev_handle) {
        libusb_release_interface(ctx->dev_handle, 0);
        libusb_close(ctx->dev_handle);
    }
    if (ctx->usb_ctx) {
        libusb_exit(ctx->usb_ctx);
    }
    free(ctx);
}

static int ch341a_probe(hexis_device_context_t* ctx, hexis_chip_info_t* detected_chip) {
    if (!ctx || !detected_chip) return -1;
    // Dummy probe: in a real implementation we would send a JEDEC ID command (9Fh) over SPI.
    // Let's pretend we detected a Winbond W25Q64JV (0xEF4017).
    const hexis_chip_info_t* info = hexis_chip_lookup_by_jedec(0xEF4017);
    if (info) {
        *detected_chip = *info;
        return 0;
    }
    return -1;
}

static int ch341a_read(hexis_device_context_t* ctx, uint64_t address, uint8_t* buffer, size_t length) {
    if (!ctx) return -1;
    (void)address;
    // Mock successful read implementation
    memset(buffer, 0xFF, length);
    return 0;
}

static int ch341a_write(hexis_device_context_t* ctx, uint64_t address, const uint8_t* buffer, size_t length) {
    if (!ctx) return -1;
    (void)address;
    (void)buffer;
    (void)length;
    // Mock successful write implementation
    return 0;
}

static int ch341a_erase(hexis_device_context_t* ctx, uint64_t address, size_t length) {
    if (!ctx) return -1;
    (void)address;
    (void)length;
    // Mock successful erase implementation
    return 0;
}

static int ch341a_verify(hexis_device_context_t* ctx, uint64_t address, const uint8_t* buffer, size_t length) {
    if (!ctx) return -1;
    (void)address;
    (void)buffer;
    (void)length;
    // Mock successful verify implementation
    return 0;
}

static int ch341a_status(hexis_device_context_t* ctx, uint8_t* status_reg, bool write) {
    if (!ctx || !status_reg) return -1;
    if (write) {
        // Write logic
    } else {
        *status_reg = 0x00; // Mock status
    }
    return 0;
}

static int ch341a_identify(hexis_device_context_t* ctx, char* identity_buf, size_t buf_len) {
    if (!ctx || !identity_buf) return -1;
    snprintf(identity_buf, buf_len, "CH341A USB SPI/I2C Programmer");
    return 0;
}

static const hexis_driver_t ch341a_driver = {
    .name = "ch341a",
    .description = "WCH CH341A USB SPI/I2C Programmer",
    .connect = ch341a_connect,
    .disconnect = ch341a_disconnect,
    .probe = ch341a_probe,
    .read = ch341a_read,
    .write = ch341a_write,
    .erase = ch341a_erase,
    .verify = ch341a_verify,
    .status = ch341a_status,
    .identify = ch341a_identify
};

int hexis_ch341a_register(void) {
    return hexis_driver_register(&ch341a_driver);
}
