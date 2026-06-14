cmake_minimum_required(VERSION 3.26)

project(hdb_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(HDB_CORE_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)

# --- Type overrides ---
# Id type: default std::string. Override example: -DHDB_ID_TYPE=uint64_t
set(HDB_ID_TYPE "" CACHE STRING "Override Id type (default: std::string)")

# TimePoint type: default std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>
# Override example: -DHDB_TIME_POINT_TYPE="std::chrono::time_point<std::chrono::steady_clock,std::chrono::nanoseconds>"
set(HDB_TIME_POINT_TYPE "" CACHE STRING "Override TimePoint type (default: system_clock / microseconds)")

# Real type: default float. Override example: -DHDB_REAL_TYPE=double
# Affects: Impulse weight, Resonance.fidelity, Thought.flux, Cortex creativity parameter.
set(HDB_REAL_TYPE "" CACHE STRING "Override real type (default: float)")

# Natural type: default std::size_t. Override example: -DHDB_NATURAL_TYPE=uint32_t
set(HDB_NATURAL_TYPE "" CACHE STRING "Override natural type (default: std::size_t)")

# Draft target: start as INTERFACE until concrete sources are added.
add_library(hdb_core INTERFACE)

# Public headers for core consumers.
target_include_directories(hdb_core
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Propagate type overrides as compile definitions.
if(HDB_ID_TYPE)
    target_compile_definitions(hdb_core INTERFACE HDB_ID_TYPE=${HDB_ID_TYPE})
endif()
if(HDB_TIME_POINT_TYPE)
    target_compile_definitions(hdb_core INTERFACE HDB_TIME_POINT_TYPE=${HDB_TIME_POINT_TYPE})
endif()
if(HDB_REAL_TYPE)
    target_compile_definitions(hdb_core INTERFACE HDB_REAL_TYPE=${HDB_REAL_TYPE})
endif()
if(HDB_NATURAL_TYPE)
    target_compile_definitions(hdb_core INTERFACE HDB_NATURAL_TYPE=${HDB_NATURAL_TYPE})
endif()

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
