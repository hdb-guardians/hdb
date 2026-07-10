cmake_minimum_required(VERSION 3.26)

project(hdb_python LANGUAGES CXX)

if(NOT TARGET hdb_sqlite)
  # Standalone python-package build path (e.g. wheel build via pyproject).
  # Reuse the same core/api/sqlite targets from the repository tree.
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../core" "${CMAKE_CURRENT_BINARY_DIR}/hdb_core")
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../api" "${CMAKE_CURRENT_BINARY_DIR}/hdb_api")
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../sqlite" "${CMAKE_CURRENT_BINARY_DIR}/hdb_sqlite")
endif()

if(NOT TARGET hdb_sqlite)
  message(FATAL_ERROR "hdb/python: failed to resolve required target hdb_sqlite")
endif()

set(Python_FIND_VIRTUALENV FIRST)
find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)

execute_process(
  COMMAND "${Python_EXECUTABLE}" -m pybind11 --cmakedir
  RESULT_VARIABLE HDB_PYBIND11_CMAKEDIR_STATUS
  OUTPUT_VARIABLE HDB_PYBIND11_CMAKEDIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
)

if(HDB_PYBIND11_CMAKEDIR_STATUS EQUAL 0 AND EXISTS "${HDB_PYBIND11_CMAKEDIR}/pybind11Config.cmake")
  list(PREPEND CMAKE_PREFIX_PATH "${HDB_PYBIND11_CMAKEDIR}")
endif()

find_package(pybind11 CONFIG REQUIRED)

add_subdirectory(bindings)

install(
  DIRECTORY src/hdb/
  DESTINATION hdb
  FILES_MATCHING
  PATTERN "*.py"
)
