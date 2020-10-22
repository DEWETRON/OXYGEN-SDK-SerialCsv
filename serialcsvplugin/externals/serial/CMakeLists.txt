FetchContent_Declare(
    serial
    GIT_REPOSITORY https://github.com/HpLightcorner/serial.git
)

FetchContent_MakeAvailable(serial)
target_include_directories(${PROJECT_NAME} PUBLIC
     ${serial_SOURCE_DIR}/include/
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    serial  
)
