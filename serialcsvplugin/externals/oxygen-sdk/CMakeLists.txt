if(NOT DEFINED ENV{OXYGEN_SDK_ROOT})
  #
  # Fetch Oxygen SDK
  message(STATUS "Fetching Oxygen-SDK...")
  FetchContent_Populate(
    oxygen
    GIT_REPOSITORY https://github.com/DEWETRON/OXYGEN-SDK.git
  )

  #
  # Fetch boost
  SET(BOOST_UNPACK_PATH  ${oxygen_SOURCE_DIR}/3rdparty/boost_1_70_0/)

  # message(STATUS "Fetching boost v1.70.0...")
  FetchContent_Populate(
    boost_1_70_0
    URL https://dl.bintray.com/boostorg/release/1.70.0/source/boost_1_70_0.zip
    SOURCE_DIR ${BOOST_UNPACK_PATH}
  )

  if(WIN32)
    #
    # Fetch Qt resource compiler
    message(STATUS "Fetching qt_resource_compiler.zip...")
    FetchContent_Populate(
      qt_resource_compiler
      URL https://github.com/DEWETRON/OXYGEN-SDK/releases/download/OXYGEN-SDK-5.3.0/qt_resource_compiler.zip
      SOURCE_DIR ${oxygen_SOURCE_DIR}/qt
    )
  endif()
  SET(OXYGEN_SDK_ROOT ${oxygen_SOURCE_DIR}/ PARENT_SCOPE )
else()
  message(STATUS "Using installed Oxygen-SDK from environment variable.")
  set(OXYGEN_SDK_ROOT $ENV{OXYGEN_SDK_ROOT} PARENT_SCOPE )
endif()
