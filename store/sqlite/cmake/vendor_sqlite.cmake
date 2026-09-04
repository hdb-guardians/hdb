include(FetchContent)

set(HDB_SQLITE_AMALGAMATION_VERSION "3530400")

FetchContent_Declare(
  hdb_sqlite3_amalgamation
  URL "https://www.sqlite.org/2026/sqlite-amalgamation-${HDB_SQLITE_AMALGAMATION_VERSION}.zip"
  URL_HASH SHA3_256=628a44cfe82c66aed1ccbbe85a562d2e33ebe64b3288981ed76285612227934e
)
FetchContent_MakeAvailable(hdb_sqlite3_amalgamation)

if(NOT CMAKE_C_COMPILER AND CMAKE_CXX_COMPILER MATCHES "clang\\+\\+")
  string(REPLACE "clang++" "clang" HDB_C_COMPILER_GUESS "${CMAKE_CXX_COMPILER}")
  if(EXISTS "${HDB_C_COMPILER_GUESS}")
    set(CMAKE_C_COMPILER "${HDB_C_COMPILER_GUESS}" CACHE FILEPATH "C compiler" FORCE)
  endif()
  unset(HDB_C_COMPILER_GUESS)
endif()
enable_language(C)

add_library(hdb_vendor_sqlite STATIC
  "${hdb_sqlite3_amalgamation_SOURCE_DIR}/sqlite3.c"
)

target_include_directories(hdb_vendor_sqlite SYSTEM PUBLIC
  "${hdb_sqlite3_amalgamation_SOURCE_DIR}"
)

set_target_properties(hdb_vendor_sqlite PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)
