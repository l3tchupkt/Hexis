if(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBUSB REQUIRED libusb-1.0)
endif()

if(WIN32)
    find_package(libusb CONFIG)
    if(libusb_FOUND)
        set(LIBUSB_INCLUDE_DIRS "")
        set(LIBUSB_LIBRARIES libusb::usb-1.0)
    else()
        find_path(LIBUSB_INCLUDE_DIR NAMES libusb.h PATH_SUFFIXES libusb-1.0)
        find_library(LIBUSB_LIBRARY NAMES usb-1.0 libusb-1.0)
        if(NOT LIBUSB_INCLUDE_DIR OR NOT LIBUSB_LIBRARY)
            message(WARNING "libusb-1.0 not found. Please use vcpkg to install libusb.")
        endif()
        set(LIBUSB_INCLUDE_DIRS ${LIBUSB_INCLUDE_DIR})
        set(LIBUSB_LIBRARIES ${LIBUSB_LIBRARY})
    endif()
endif()

add_library(hexis SHARED
    src/hexis.c
    src/hexis_chip.c
    src/hexis_driver.c
    src/plugin.c
    src/hexis_firmware.c
    src/hexis_parser.c
    src/hexis_event.c
    src/hexis_job.c
    src/hexis_graph.c
    src/hexis_patch.c
    src/hexis_vfs.c
    src/hexis_session.c
    src/crypto/sha256.c
    src/project/hexis_cas.c
    src/project/hexis_project.c
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

if(WIN32)
    target_link_libraries(hexis PRIVATE
        ${LIBUSB_LIBRARIES}
    )
else()
    target_link_libraries(hexis PRIVATE
        ${LIBUSB_LIBRARIES}
        ${CMAKE_DL_LIBS}
        m
    )
endif()


# In later phases, we will add driver source files and sub-libraries.
