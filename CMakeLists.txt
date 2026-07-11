cmake_minimum_required(VERSION 3.26)

project(hdb LANGUAGES CXX)

option(HDB_BUILD_SQLITE "Build SQLite-backed runtime" ON)

add_subdirectory(core)
add_subdirectory(api)
add_subdirectory(python)

if(HDB_BUILD_SQLITE)
  add_subdirectory(sqlite)
endif()
