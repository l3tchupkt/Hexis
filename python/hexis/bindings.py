import os
import sys
from cffi import FFI

ffi = FFI()

# Define the C declarations for Hexis
ffi.cdef("""
    // from hexis.h
    int hexis_init(void);
    const char* hexis_get_version(void);

    // from hexis_chip.h
    typedef enum {
        HEXIS_MEM_TYPE_UNKNOWN = 0,
        HEXIS_MEM_TYPE_SPI_NOR,
        HEXIS_MEM_TYPE_SPI_NAND,
        HEXIS_MEM_TYPE_I2C_EEPROM,
        HEXIS_MEM_TYPE_PARALLEL_NOR,
        HEXIS_MEM_TYPE_PARALLEL_NAND
    } hexis_mem_type_t;

    typedef struct {
        const char* manufacturer;
        const char* model;
        uint64_t capacity_bytes;
        uint32_t sector_size;
        uint32_t page_size;
        float voltage;
        uint32_t jedec_id;
        uint32_t supported_operations;
        hexis_mem_type_t memory_type;
    } hexis_chip_info_t;

    // from hexis_driver.h
    typedef struct hexis_device_context hexis_device_context_t;

    typedef struct {
        const char* name;
        const char* description;
        int (*connect)(hexis_device_context_t** ctx);
        void (*disconnect)(hexis_device_context_t* ctx);
        int (*probe)(hexis_device_context_t* ctx, hexis_chip_info_t* detected_chip);
        int (*read)(hexis_device_context_t* ctx, uint64_t address, uint8_t* buffer, size_t length);
        int (*write)(hexis_device_context_t* ctx, uint64_t address, const uint8_t* buffer, size_t length);
        int (*erase)(hexis_device_context_t* ctx, uint64_t address, size_t length);
        int (*verify)(hexis_device_context_t* ctx, uint64_t address, const uint8_t* buffer, size_t length);
        int (*status)(hexis_device_context_t* ctx, uint8_t* status_reg, bool write);
        int (*identify)(hexis_device_context_t* ctx, char* identity_buf, size_t buf_len);
    } hexis_driver_t;

    const hexis_driver_t* hexis_driver_get(const char* name);
""")

def _load_library():
    # Attempt to locate the compiled libhexis shared library
    search_paths = [
        # When running in tree (build dir)
        os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "build", "libhexis")),
        os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "build", "libhexis", "Debug")),
        os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "build", "libhexis", "Release")),
        # System paths or local directory
        os.path.dirname(__file__),
        "."
    ]
    
    lib_name = "libhexis.so"
    if sys.platform == "win32":
        lib_name = "hexis.dll"
    elif sys.platform == "darwin":
        lib_name = "libhexis.dylib"
        
    for path in search_paths:
        full_path = os.path.join(path, lib_name)
        if os.path.exists(full_path):
            return ffi.dlopen(full_path)
            
    # Fallback to standard system library path resolution
    try:
        return ffi.dlopen(lib_name)
    except OSError:
        pass
        
    raise RuntimeError(f"Could not find or load {lib_name}")

lib = _load_library()

# Initialize core library immediately on import
if lib.hexis_init() != 0:
    raise RuntimeError("Failed to initialize Hexis C core")
