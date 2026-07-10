pybind11_add_module(_hdb MODULE module.cpp)

target_link_libraries(_hdb PRIVATE hdb_api)

target_compile_features(_hdb PRIVATE cxx_std_23)
set_target_properties(_hdb PROPERTIES CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(_hdb PRIVATE -Wall -Wextra)
elseif(MSVC)
  target_compile_options(_hdb PRIVATE /W4)
endif()

install(
  TARGETS _hdb
  LIBRARY DESTINATION hdb
  RUNTIME DESTINATION hdb
)
