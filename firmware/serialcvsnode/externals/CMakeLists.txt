
#
# Which ETL to use

set(PULL_CURRENT_ETL 0)


if (PULL_CURRENT_ETL)

  FetchContent_Populate(
      etl
      GIT_REPOSITORY https://github.com/ETLCPP/etl.git
    )

  target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/etl-src/include/
  )

else()

  target_include_directories(${PROJECT_NAME}
    PUBLIC etl/include/
  )

endif()
