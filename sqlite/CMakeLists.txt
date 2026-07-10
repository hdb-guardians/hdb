cmake_minimum_required(VERSION 3.26)

project(hdb_sqlite LANGUAGES CXX)

if(NOT TARGET hdb_core)
  message(FATAL_ERROR "hdb/sqlite: required target hdb_core is missing. hint: add_subdirectory(core) before sqlite")
endif()

find_package(SQLite3 REQUIRED)

add_library(hdb_sqlite STATIC
  src/context.cpp
  src/neuron_table.cpp
  src/synapse_table.cpp
  src/dream_table.cpp
  src/store.cpp
)

target_include_directories(hdb_sqlite
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(hdb_sqlite
  PUBLIC
    hdb_core
  PRIVATE
    SQLite3::SQLite3
)

target_compile_features(hdb_sqlite PUBLIC cxx_std_23)
set_target_properties(hdb_sqlite PROPERTIES CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(hdb_sqlite PRIVATE
    -Wall -Wextra -Wpedantic
  )
elseif(MSVC)
  target_compile_options(hdb_sqlite PRIVATE /W4)
endif()
