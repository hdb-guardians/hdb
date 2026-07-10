pybind11_add_module(_hdb MODULE module.cpp)

target_link_libraries(_hdb PRIVATE hdb_api)

target_compile_features(_hdb PRIVATE cxx_std_23)
set_target_properties(_hdb PROPERTIES CXX_EXTENSIONS OFF)

install(
  TARGETS _hdb
  LIBRARY DESTINATION hdb
  RUNTIME DESTINATION hdb
)
