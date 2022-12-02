cmake_minimum_required(VERSION 3.16)

project(serialcsv)


#
# Default: Disable Plugin/Firmware-Tests
option(BUILD_PLUGIN_WITH_TESTS "Build Unit-Tests." OFF)

if(BUILD_PLUGIN_WITH_TESTS)
    # Ensure CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS is TRUE when Building with tests
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

add_subdirectory(serialcsvplugin)


if(BUILD_PLUGIN_WITH_TESTS)
    add_subdirectory(test)
#     add_dependencies(serialcsvplugin_test serialcsvplugin)
endif()
