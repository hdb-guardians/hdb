cmake_minimum_required(VERSION 3.26)

project(hdb_standard LANGUAGES CXX)

if(NOT TARGET hdb_core)
  message(FATAL_ERROR "hdb/standard: required target hdb_core is missing. hint: add_subdirectory(core) before standard")
endif()

find_package(SQLite3 REQUIRED)

add_library(hdb_standard STATIC
  src/sqlite_context.cpp
  src/sqlite_neuron_table.cpp
  src/sqlite_synapse_table.cpp
  src/sqlite_dream_table.cpp
  src/session.cpp
)

target_include_directories(hdb_standard
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(hdb_standard
  PUBLIC
    hdb_core
  PRIVATE
    SQLite3::SQLite3
)

target_compile_features(hdb_standard PUBLIC cxx_std_23)
set_target_properties(hdb_standard PROPERTIES CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(hdb_standard PRIVATE
    -Wall -Wextra -Wpedantic
  )
elseif(MSVC)
  target_compile_options(hdb_standard PRIVATE /W4)
endif()
