cmake_minimum_required(VERSION 3.17)
project(warframe
  VERSION 0.0.1
  LANGUAGES C
)

# Create this project as a library
add_library(${PROJECT_NAME} SHARED
  src/wfm.c
  src/wf.c
  src/network.c

  include/warframe.h
  include/network.h
  include/worldstate.h
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


set(BUILD_CURL_EXE OFF)

add_subdirectory(lib/curl)
add_subdirectory(lib/cJSON)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/cJSON)
target_link_libraries(${PROJECT_NAME} libcurl cjson)


#include(GNUInstallDirs)
#configure_file(warframe.pc.in warframe.pc @ONLY)
#install(TARGETS ${PROJECT_NAME}
#  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#)
#install(FILES ${CMAKE_BINARY_DIR}/warframe.pc
#  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR/pkgconfig
#)
