pybind11_add_module(_hdb_api MODULE module.cpp)

target_link_libraries(_hdb_api PRIVATE hdb_api)

target_include_directories(
  _hdb_api PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_compile_features(_hdb_api PRIVATE cxx_std_23)
set_target_properties(_hdb_api PROPERTIES CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(_hdb_api PRIVATE -Wall -Wextra)
elseif(MSVC)
  target_compile_options(_hdb_api PRIVATE /W4)
endif()

install(
  TARGETS _hdb_api
  LIBRARY DESTINATION hdb
  RUNTIME DESTINATION hdb
)
