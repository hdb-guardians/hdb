cmake_minimum_required(VERSION 3.26)

project(hdb LANGUAGES CXX)

option(HDB_BUILD_STANDARD "Build standard SQLite-backed runtime" ON)
option(HDB_BUILD_PYTHON "Build Python bindings" OFF)

add_subdirectory(core)

if(HDB_BUILD_STANDARD)
  add_subdirectory(standard)
endif()

if(HDB_BUILD_PYTHON)
  add_subdirectory(python)
endif()
