cmake_minimum_required(VERSION 3.26)

project(hdb_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(HDB_CORE_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)

# Draft target: start as INTERFACE until concrete sources are added.
add_library(hdb_core INTERFACE)

# Public headers for core consumers.
target_include_directories(hdb_core
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Prefer strict diagnostics when using clang.
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(hdb_core
        INTERFACE
            -Wall
            -Wextra
            -Wpedantic
    )

    if(HDB_CORE_WARNINGS_AS_ERRORS)
        target_compile_options(hdb_core INTERFACE -Werror)
    endif()
endif()

# Dependency placeholders (enable once package names/targets are finalized).
# find_package(SQLite3 REQUIRED)
# find_package(kuzu CONFIG REQUIRED)
# find_package(sqlite-vec CONFIG REQUIRED)
# target_link_libraries(hdb_core INTERFACE SQLite::SQLite3 kuzu::kuzu sqlite-vec::sqlite-vec)
