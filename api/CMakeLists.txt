cmake_minimum_required(VERSION 3.26)

project(hdb_api LANGUAGES CXX)

if(NOT TARGET hdb_core)
  message(FATAL_ERROR "hdb/api: required target hdb_core is missing. hint: add_subdirectory(core) before api")
endif()

add_library(hdb_api STATIC
  src/session.cpp
)

target_include_directories(hdb_api
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(hdb_api
  PUBLIC
    hdb_core
)

target_compile_features(hdb_api PUBLIC cxx_std_23)
set_target_properties(hdb_api PROPERTIES CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(hdb_api PRIVATE
    -Wall -Wextra -Wpedantic
  )
elseif(MSVC)
  target_compile_options(hdb_api PRIVATE /W4)
endif()
