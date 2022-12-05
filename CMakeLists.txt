cmake_minimum_required(VERSION 3.16)

project(serialcsv)


#
# Default: Disable Plugin/Firmware-Tests
#option(BUILD_PLUGIN_WITH_TESTS "Build Unit-Tests." OFF)
option(BUILD_PLUGIN_WITH_TESTS "Build Unit-Tests." ON)

if(BUILD_PLUGIN_WITH_TESTS)
    # Ensure CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS is TRUE when Building with tests
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    enable_testing()
endif()

#
## Oxygen SDK start
set(CMAKE_CXX_STANDARD 17)

if(NOT EXISTS ${OXYGEN_SDK_ROOT})
    if (EXISTS ${CMAKE_SOURCE_DIR}/../OXYGEN-SDK)
        set(OXYGEN_SDK_ROOT ${CMAKE_SOURCE_DIR}/../OXYGEN-SDK)
    else()
        message(FATAL_ERROR "Could not find OXYGEN-SDK!\nEither install it parallel to the plugins directory or set OXYGEN_SDK_ROOT to a valid path")
    endif()    
endif()

set(ODK_ROOT ${OXYGEN_SDK_ROOT})
message(STATUS "ODKROOT = ${ODK_ROOT}")

# expand cmake search path to check for project settings
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ODK_ROOT}/cmake )

include(CMakeSettings)
include(OxygenPluginFunctions)

# Set Static Runtime 
set(MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

SetCommonOutputDirectory()
## Oxygen SDK end


add_subdirectory(serialcsvplugin)


if(BUILD_PLUGIN_WITH_TESTS)
    add_subdirectory(test)
#     add_dependencies(serialcsvplugin_test serialcsvplugin)
endif()
