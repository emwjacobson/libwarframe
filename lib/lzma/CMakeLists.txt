cmake_minimum_required(VERSION 3.0)
project(lzma
  VERSION 23.01
  LANGUAGES C
)

# Create this project as a library
add_library(${PROJECT_NAME} STATIC
    LzmaDec.h
    7zTypes.h

    LzmaDec.c
)

# Set version and public header
set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 1
  PUBLIC_HEADER "LzmaDec.h"
)

target_include_directories(${PROJECT_NAME} PUBLIC
  .
)
