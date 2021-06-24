cmake_minimum_required(VERSION 3.19.2)
project(cproperties)

add_library(cproperties STATIC
    src/cproperties.cpp
)

target_include_directories( cproperties
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)