#
# Oxygen Modbus-Plugin
# Queries a Modbus-Context and samples it for oxygen
#
cmake_minimum_required(VERSION 3.16)

#
# Include some helpers and the fetchcontent module for external dependencies
include(FetchContent)

#
# Define file-generate for relative paths
if(POLICY CMP0070)
   cmake_policy(SET CMP0070 NEW)
endif()

#
# Activate Policy for Static-Runtime 
if(POLICY CMP0091)
   cmake_policy(SET CMP0091 NEW)
endif()

#
# Activate Policy for option() honors normal variables
if(POLICY CMP0077)
   cmake_policy(SET CMP0077 NEW)
endif()

#
# Project
project(serialcsvplugin)

#
# C++ Standard, Compiler Flags
set(CMAKE_CXX_STANDARD 17)

if( MSVC )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

#
# Oxygen SDK
add_subdirectory(externals/oxygen-sdk)
set(ODK_ROOT ${OXYGEN_SDK_ROOT})
message("ODKROOT = ${ODK_ROOT}")

# expand cmake search path to check for project settings
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ODK_ROOT}/cmake )

include(CMakeSettings)
include(OxygenPluginFunctions)

# Set Static Runtime 
set(MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

SetCommonOutputDirectory()
SetBoostOptions()
find_package(Boost REQUIRED)
find_package(SDKQt REQUIRED)

message(STATUS "Qt5Core_RCC_EXECUTABLE = ${Qt5Core_RCC_EXECUTABLE}")

AddUniqueTargetFromSubdirectory(pugixml "${SW_APP_ROOT}/3rdparty/pugixml-1.9/scripts" "3rdparty/pugixml-1.9")
AddUniqueTargetFromSubdirectory(odk "${ODK_ROOT}/odk" "odk")

set(SERIAL_PLUGIN_HEADER_FILES
    include/serialcsvplugin/details/common.h
    include/serialcsvplugin/CsvDataLineDecoder.h
    include/serialcsvplugin/CsvChannel.h
    include/serialcsvplugin/details/SerialPort.h
    include/serialcsvplugin/CsvAdvancedProtocolLineDecoder.h
)
source_group("Header Files" FILES ${SERIAL_PLUGIN_HEADER_FILES})

set(SERIAL_PLUGIN_SOURCE_FILES
    src/CsvDataLineDecoder.cpp
    src/SerialPort.cpp
    src/SerialCsvPlugin.cpp
    src/CsvAdvancedProtocolLineDecoder.cpp
    src/CsvDecoder.cpp
)
source_group("Source Files" FILES ${SERIAL_PLUGIN_SOURCE_FILES})

set(SERIAL_PLUGIN_QML_FILES
    qml/AddSerialCsvPort.qml
)
source_group("QML Files" FILES ${SERIAL_PLUGIN_QML_FILES})


add_library(${PROJECT_NAME} SHARED
    ${SERIAL_PLUGIN_HEADERS}
    ${SERIAL_PLUGIN_SOURCE_FILES}
    ${SERIAL_PLUGIN_QML_FILES}
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE odk_framework
)

target_include_directories(${PROJECT_NAME}
    PUBLIC include
    SYSTEM ${Boost_INCLUDE_DIRS}
)

#
# Add QML Resources
qt5_add_binary_resources(${PROJECT_NAME}_qml qml.qrc DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/qml.rcc")

set(RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/resources)
target_include_directories(${PROJECT_NAME}
        PRIVATE ${RESOURCE_DIR}
)
AddResourceFile(${PROJECT_NAME} "${CMAKE_CURRENT_BINARY_DIR}/qml.rcc" ${RESOURCE_DIR})

#
# Add Dependencies 
add_subdirectory(externals)

#
# General-Plugin Options
SetPluginOutputOptions(${PROJECT_NAME})
