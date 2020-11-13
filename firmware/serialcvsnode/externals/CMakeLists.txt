
FetchContent_Populate(
    etl
    GIT_REPOSITORY https://github.com/ETLCPP/etl.git
  )

#message(FATAL_ERROR "${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/etl-src/include/
)
