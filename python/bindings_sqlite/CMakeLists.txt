pybind11_add_module(_hdb_sqlite MODULE module.cpp)

target_link_libraries(_hdb_sqlite PRIVATE hdb_sqlite)

target_compile_features(_hdb_sqlite PRIVATE cxx_std_23)
set_target_properties(_hdb_sqlite PROPERTIES CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(_hdb_sqlite PRIVATE -Wall -Wextra)
elseif(MSVC)
  target_compile_options(_hdb_sqlite PRIVATE /W4)
endif()

install(
  TARGETS _hdb_sqlite
  LIBRARY DESTINATION hdb
  RUNTIME DESTINATION hdb
)
