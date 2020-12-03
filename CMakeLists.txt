cmake_minimum_required(VERSION 3.16)

project(serialcsv)

#
# Default: Disable the RE2 test
option(RE2_BUILD_TESTING OFF)

#
# Default: Disable Plugin/Firmware-Tests
option(BUILD_PLUGIN_WITH_TESTS "Build Unit-Tests for both, the embedded-firmware and the Oxygen-Plugin." OFF)

if(BUILD_PLUGIN_WITH_TESTS)
    # Ensure CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS is TRUE when Building with tests
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

add_subdirectory(serialcsvplugin)
add_subdirectory(firmware)

if(BUILD_PLUGIN_WITH_TESTS)
    add_subdirectory(test)
    add_dependencies(serialcsvplugin_test serialcsvplugin embeddedcsvnode)
endif()
