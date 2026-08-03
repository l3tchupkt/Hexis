find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

add_library(hexis SHARED
    src/hexis.c
    src/hexis_chip.c
    src/hexis_driver.c
    src/plugin.c
    src/hexis_firmware.c
    src/hexis_parser.c
    src/parsers/elf.c
    drivers/ch341a/ch341a.c
    firmware/analyzer.c
    firmware/scanner.c
)

target_include_directories(hexis PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(hexis PRIVATE
    ${LIBUSB_INCLUDE_DIRS}
)

target_link_libraries(hexis PRIVATE
    ${LIBUSB_LIBRARIES}
    ${CMAKE_DL_LIBS}
    m
)


# In later phases, we will add driver source files and sub-libraries.
