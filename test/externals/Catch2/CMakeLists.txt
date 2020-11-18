FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v2.13.1
)


FetchContent_MakeAvailable(Catch2)
target_include_directories(${PROJECT_NAME} PRIVATE
     ${Catch2_SOURCE_DIR}/single_include/
)
