cmake_minimum_required(VERSION 3.26)

project(hdb LANGUAGES CXX)

option(HDB_BUILD_SQLITE "Build SQLite-backed store" ON)

add_subdirectory(core)

if(HDB_BUILD_SQLITE)
  add_subdirectory(store/sqlite)
endif()

add_subdirectory(python)
