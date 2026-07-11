cmake_minimum_required(VERSION 3.26)

project(hdb_core LANGUAGES CXX)

add_library(hdb_core STATIC
  src/molecule/prefrontal.cpp
  src/molecule/thalamus.cpp
  src/molecule/hippocampus.cpp
  src/molecule/cortex.cpp
)

target_include_directories(hdb_core
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(hdb_core PUBLIC cxx_std_23)
set_target_properties(hdb_core PROPERTIES CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(hdb_core PRIVATE
    -Wall -Wextra -Wpedantic -Werror
  )
elseif(MSVC)
  target_compile_options(hdb_core PRIVATE /W4 /WX)
endif()