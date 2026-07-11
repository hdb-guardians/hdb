cmake_minimum_required(VERSION 3.26)

project(hdb_python LANGUAGES CXX)

# Standalone build path (e.g. wheel build via pyproject).
if(NOT TARGET hdb_core)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../core" "${CMAKE_CURRENT_BINARY_DIR}/hdb_core")
endif()
if(NOT TARGET hdb_sqlite)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../store/sqlite" "${CMAKE_CURRENT_BINARY_DIR}/hdb_sqlite")
endif()

# Auto-detect venv Python executable if not explicitly provided via -DPython_EXECUTABLE=...
if(NOT Python_EXECUTABLE)
  if(WIN32)
    set(_hdb_venv_python "${CMAKE_SOURCE_DIR}/python/.venv/Scripts/python.exe")
  else()
    set(_hdb_venv_python "${CMAKE_SOURCE_DIR}/python/.venv/bin/python")
  endif()
  if(EXISTS "${_hdb_venv_python}")
    set(Python_EXECUTABLE "${_hdb_venv_python}" CACHE FILEPATH "Python interpreter" FORCE)
  endif()
  unset(_hdb_venv_python)
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

add_subdirectory(bindings_core)

if(TARGET hdb_sqlite)
  add_subdirectory(bindings_sqlite)
endif()

install(
  DIRECTORY src/hdb/
  DESTINATION hdb
  FILES_MATCHING
  PATTERN "*.py"
  PATTERN "*.pyi"
)
