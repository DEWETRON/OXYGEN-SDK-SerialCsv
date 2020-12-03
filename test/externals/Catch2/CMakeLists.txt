FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v2.13.1
)


FetchContent_MakeAvailable(Catch2)
target_include_directories(${PROJECT_NAME} PRIVATE
     ${Catch2_SOURCE_DIR}/single_include/
)

target_link_libraries(${PROJECT_NAME} Catch2::Catch2)
