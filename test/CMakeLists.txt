cmake_minimum_required(VERSION 3.16)

project(serialcsvplugin_test)
set(CMAKE_CXX_STANDARD 20)

option(BUILD_SERIAL_PORT_INTEGRATION_TEST "Build Serial-Port Tests if integration tests can be executed" OFF)


# if( MSVC )
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
# else()
# 	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
# endif()


find_package(Boost     REQUIRED COMPONENTS
    unit_test_framework
)

add_executable(
        ${PROJECT_NAME}
        main.cpp
        TestCsvDataLineDecoder.cpp
        # TestFirmware.cpp
        # TestFirmwareErrors.cpp
        # TestFirmwareExample.cpp
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

target_link_libraries(${PROJECT_NAME} serialcsvplugin)
target_link_libraries(${PROJECT_NAME} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

#
# Register Tests for CTest
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${Catch2_SOURCE_DIR}/contrib/)
# include(CTest)
# include(ParseAndAddCatchTests)

# message(STATUS "When using CTest, tests will be executed in the following working directory:")
# message(STATUS "${OXYGEN_PLUGIN_OUTPUT_DIRECTORY_GENERATOR_EXPR}")
# message(STATUS "Generator-Expressions will be handeld by CTest.")

# set(AdditionalCatchParameters WORKING_DIRECTORY ${OXYGEN_PLUGIN_OUTPUT_DIRECTORY_GENERATOR_EXPR}) 
# ParseAndAddCatchTests(${PROJECT_NAME})
