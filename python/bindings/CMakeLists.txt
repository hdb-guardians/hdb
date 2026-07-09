pybind11_add_module(_hdb MODULE module.cpp)

target_link_libraries(_hdb PRIVATE hdb_standard)

target_compile_features(_hdb PRIVATE cxx_std_23)
set_target_properties(_hdb PROPERTIES CXX_EXTENSIONS OFF)
