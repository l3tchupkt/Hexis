# Hexis C API Reference

## Core Initialization
```c
int hexis_init(void);
void hexis_exit(void);
```

## Driver API
Drivers implement `hexis_driver_t` and register using `hexis_driver_register()`.

```c
typedef struct {
    const char* name;
    const char* description;
    
    int (*init)(void);
    void (*cleanup)(void);
    void* (*open_device)(void);
    void (*close_device)(void* handle);
    int (*spi_transfer)(void* handle, const uint8_t* tx_data, uint8_t* rx_data, size_t length);
} hexis_driver_t;
```

## Firmware Analysis
```c
int hexis_fw_analyze(const uint8_t* buffer, size_t length, hexis_fw_analysis_t** out_analysis);
double hexis_fw_calculate_entropy(const uint8_t* buffer, size_t length);
void hexis_fw_analysis_free(hexis_fw_analysis_t* analysis);
```

## Security Scanner
```c
int hexis_fw_scan_secrets(const uint8_t* buffer, size_t length, hexis_scan_report_t** out_report);
void hexis_scan_report_free(hexis_scan_report_t* report);
```

## Plugin System
```c
int hexis_plugin_load(const char* path);
```
Plugins must export `hexis_plugin_entry()`.
