cmake_minimum_required(VERSION 3.17)
project(warframe
  VERSION 0.0.1
  LANGUAGES C
)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_C_FLAGS_DEBUG "-g -DPRINT_LEVEL=DEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DPRINT_LEVEL=ERROR")

# Create this project as a library
add_library(${PROJECT_NAME} STATIC
  src/wfm.c
  src/wf.c
  src/network.c

  include/wf_defs.h
  include/warframe.h
  include/network.h
  include/worldstate.h
  include/debug.h
)

# Set version and public header
set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 1
  PUBLIC_HEADER include/warframe.h
)

# Include directory
target_include_directories(${PROJECT_NAME} PUBLIC
  include/
)

set(BUILD_SHARED_AND_STATIC_LIBS ON CACHE BOOL "Enable cJSON Static Library")
set(ENABLE_CJSON_TEST OFF CACHE BOOL "Enable building cJSON test")

find_package(CURL REQUIRED)
add_subdirectory(lib/cJSON)
  target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/cJSON)
add_subdirectory(lib/lzma)
target_link_libraries(${PROJECT_NAME} curl cjson-static lzma)
