FetchContent_Populate(
    jthread
    GIT_REPOSITORY https://github.com/josuttis/jthread.git
)

target_include_directories(${PROJECT_NAME} PUBLIC
     ${jthread_SOURCE_DIR}/source/
)
