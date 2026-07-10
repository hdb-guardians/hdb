#include <pybind11/pybind11.h>

#include <hdb/sqlite/open.hpp>
#include <string>

namespace py = pybind11;

PYBIND11_MODULE(_hdb_sqlite, m) {
  m.doc() = "HDB SQLite backend bindings";

  m.def(
      "open_sqlite",
      [](const std::string& db_path, const std::string& vec_ext) {
        auto [neurons, synapses, dreams] = hdb::sqlite::open_sqlite(
            db_path, hdb::sqlite::resolve_vec_extension_path(vec_ext));
        return py::make_tuple(neurons, synapses, dreams);
      },
      py::arg("db_path"),
      py::arg("sqlite_vec_extension_path") = "");
}
