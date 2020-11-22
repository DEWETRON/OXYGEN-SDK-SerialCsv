cmake_minimum_required(VERSION 3.16)
Include(FetchContent)

project(embeddedcsvnode)

if( MSVC )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

add_library(
        ${PROJECT_NAME}
        src/CsvNode.cpp
        src/details/Registry.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC src/
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE CSVNODE_RUNNING_ON_OS
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
add_subdirectory(externals)
