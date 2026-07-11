pybind11_add_module(_hdb_core MODULE module.cpp)

target_link_libraries(_hdb_core PRIVATE hdb_core)

target_include_directories(
  _hdb_core PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_compile_features(_hdb_core PRIVATE cxx_std_23)
set_target_properties(_hdb_core PROPERTIES CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(_hdb_core PRIVATE -Wall -Wextra)
elseif(MSVC)
  target_compile_options(_hdb_core PRIVATE /W4)
endif()

install(
  TARGETS _hdb_core
  LIBRARY DESTINATION hdb
  RUNTIME DESTINATION hdb
)
