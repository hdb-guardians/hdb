cmake_minimum_required(VERSION 3.26)

project(hdb_python LANGUAGES CXX)

if(NOT TARGET hdb_standard)
  message(FATAL_ERROR "hdb_standard target is required. Enable HDB_BUILD_STANDARD.")
endif()

find_package(pybind11 CONFIG REQUIRED)

add_subdirectory(bindings)
