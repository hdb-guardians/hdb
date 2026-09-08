#include <pybind11/pybind11.h>

#include <cstddef>
#include <hdb/sqlite/open.hpp>
#include <string>

namespace py = pybind11;

PYBIND11_MODULE(_hdb_sqlite, m) {
  m.doc() = "HDB SQLite backend bindings";

  m.def(
      "open_sqlite",
      [](const std::string& db_path,
         const std::string& vec_ext,
         const std::size_t dream_dimension) {
        auto [neurons, synapses, dreams] = hdb::sqlite::open_sqlite(
            db_path,
            hdb::sqlite::resolve_vec_extension_path(vec_ext),
            dream_dimension);
        return py::make_tuple(neurons, synapses, dreams);
      },
      py::arg("db_path") = "human.db",
      py::arg("sqlite_vec_extension_path") = "",
      py::arg("dream_dimension"));
}
