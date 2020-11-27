cmake_minimum_required(VERSION 3.16)
Include(FetchContent)

project(embeddedcsvnode)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE src/
)

if(BUILD_PLUGIN_WITH_TESTS)
    message(STATUS "CSVNode will throw exceptions when running tests.")
    target_compile_definitions(${PROJECT_NAME}
        INTERFACE CSVNODE_THROW_EXCEPTION
    )
endif()

add_subdirectory(externals)
