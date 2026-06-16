cmake_minimum_required(VERSION 3.26)

project(hdb_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(HDB_CORE_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)

# --- Type overrides ---
# Id type: default std::string. Override example: -DHDB_ID_TYPE=uint64_t
set(HDB_ID_TYPE "" CACHE STRING "Override Id type (default: std::string)")

# Moment type: default Clock::time_point.
# Override example: -DHDB_MOMENT_TYPE="std::chrono::time_point<std::chrono::steady_clock,std::chrono::nanoseconds>"
set(HDB_MOMENT_TYPE "" CACHE STRING "Override Moment type (default: Clock::time_point)")

# Real type: default float. Override example: -DHDB_REAL_TYPE=double
# Affects: Impulse weight, Resonance.fidelity, Thought.flux, Cortex creativity parameter.
set(HDB_REAL_TYPE "" CACHE STRING "Override real type (default: float)")

# Natural type: default std::size_t. Override example: -DHDB_NATURAL_TYPE=uint32_t
set(HDB_NATURAL_TYPE "" CACHE STRING "Override natural type (default: std::size_t)")

add_library(hdb_core STATIC
    src/molecule/prefrontal.cpp
    src/molecule/thalamus.cpp
    src/molecule/hippocampus.cpp
    src/molecule/cortex.cpp
)

# Public headers for consumers; private src/ for internal detail headers.
target_include_directories(hdb_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Propagate type overrides so consumers see the same types.
if(HDB_ID_TYPE)
    target_compile_definitions(hdb_core PUBLIC HDB_ID_TYPE=${HDB_ID_TYPE})
endif()
if(HDB_MOMENT_TYPE)
    target_compile_definitions(hdb_core PUBLIC HDB_MOMENT_TYPE=${HDB_MOMENT_TYPE})
endif()
if(HDB_REAL_TYPE)
    target_compile_definitions(hdb_core PUBLIC HDB_REAL_TYPE=${HDB_REAL_TYPE})
endif()
if(HDB_NATURAL_TYPE)
    target_compile_definitions(hdb_core PUBLIC HDB_NATURAL_TYPE=${HDB_NATURAL_TYPE})
endif()

# Strict diagnostics (private — only for building hdb_core itself).
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

# Store implementations (SQLite, sqlite-vec, etc.) live outside core.
# Add them in a separate CMake project that links against hdb_core.
