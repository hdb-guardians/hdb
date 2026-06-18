cmake_minimum_required(VERSION 3.26)

project(hdb_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(HDB_CORE_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)

set(HDB_ID_TYPE "" CACHE STRING "Override Id type (default: std::string)")

set(HDB_CLOCK_TYPE "" CACHE STRING "Override Clock type (default: std::chrono::system_clock)")

set(HDB_REAL_TYPE "" CACHE STRING "Override real type (default: float)")

set(HDB_NATURAL_TYPE "" CACHE STRING "Override natural type (default: std::size_t)")

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

if(HDB_ID_TYPE)
    target_compile_definitions(hdb_core PUBLIC HDB_ID_TYPE=${HDB_ID_TYPE})
endif()
if(HDB_CLOCK_TYPE)
    target_compile_definitions(hdb_core PUBLIC HDB_CLOCK_TYPE=${HDB_CLOCK_TYPE})
endif()
if(HDB_REAL_TYPE)
    target_compile_definitions(hdb_core PUBLIC HDB_REAL_TYPE=${HDB_REAL_TYPE})
endif()
if(HDB_NATURAL_TYPE)
    target_compile_definitions(hdb_core PUBLIC HDB_NATURAL_TYPE=${HDB_NATURAL_TYPE})
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(hdb_core PRIVATE
        -Wall -Wextra -Wpedantic
    )
    if(HDB_CORE_WARNINGS_AS_ERRORS)
        target_compile_options(hdb_core PRIVATE -Werror)
    endif()
elseif(MSVC)
    target_compile_options(hdb_core PRIVATE /W4)
    if(HDB_CORE_WARNINGS_AS_ERRORS)
        target_compile_options(hdb_core PRIVATE /WX)
    endif()
endif()