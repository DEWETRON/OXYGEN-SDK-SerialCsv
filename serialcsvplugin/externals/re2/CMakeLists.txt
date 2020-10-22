FetchContent_Declare(
    re2
    GIT_REPOSITORY https://github.com/google/re2.git
)

set(RE2_BUILD_TESTING OFF)
FetchContent_MakeAvailable(re2)
target_include_directories(${PROJECT_NAME} PRIVATE
     ${re2_SOURCE_DIR}/
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    re2
)
