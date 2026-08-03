add_library(hexis SHARED
    src/hexis.c
    src/hexis_chip.c
    src/hexis_driver.c
)

target_include_directories(hexis PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# In later phases, we will add driver source files and sub-libraries.
