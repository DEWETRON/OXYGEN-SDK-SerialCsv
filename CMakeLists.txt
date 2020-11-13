cmake_minimum_required(VERSION 3.16)

project(serialcsv)

#
# Default: Disable the RE2 test
option(RE2_BUILD_TESTING OFF)

add_subdirectory(serialcsvplugin)
add_subdirectory(firmware)
#add_subdirectory(test)

#add_dependencies(serialcsvplugin_test serialcsvplugin serialcsvnode)
