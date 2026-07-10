cmake_minimum_required(VERSION 3.26)

project(hdb LANGUAGES CXX)

option(HDB_BUILD_SQLITE "Build SQLite-backed runtime" ON)
option(HDB_BUILD_PYTHON "Build Python bindings" ON)

add_subdirectory(core)
add_subdirectory(api)

if(HDB_BUILD_SQLITE)
  add_subdirectory(sqlite)
endif()

if(HDB_BUILD_PYTHON)
  add_subdirectory(python)
endif()
