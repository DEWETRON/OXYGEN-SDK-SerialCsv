cmake_minimum_required(VERSION 3.17)
Include(FetchContent)

project(serialcsvplugin_test)
set(CMAKE_CXX_STANDARD 17)

option(BUILD_SERIAL_PORT_INTEGRATION_TEST "Build Serial-Port Tests if integration tests can be executed" OFF)

if( MSVC )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

add_executable(
        ${PROJECT_NAME}
        main.cpp
        TestCsvDataLineDecoder.cpp
        TestFirmware.cpp
        TestCsvAdvancedProtocolLineDecoder.cpp
)

if (BUILD_SERIAL_PORT_INTEGRATION_TEST)
    message(STATUS "Building Serial-Port Integrationtests.")
    target_sources(
        ${PROJECT_NAME}
        PRIVATE OptionalIntegrationTestSerialPort.cpp
    )
endif()

#
# Add Dependencies
add_subdirectory(externals)
target_link_libraries(${PROJECT_NAME} serialcsvplugin)
target_link_libraries(${PROJECT_NAME} embeddedcsvnode)
