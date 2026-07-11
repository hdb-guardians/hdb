#include <pybind11/chrono.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cstddef>
#include <memory>
#include <optional>
#include <span>
#include <vector>

#include <hdb/atom/engram.hpp>
#include <hdb/atom/thought.hpp>
#include <hdb/python/byte_caster.hpp>
#include <hdb/quark/dream.hpp>
#include <hdb/quark/neuron.hpp>
#include <hdb/quark/resonance.hpp>
#include <hdb/quark/synapse.hpp>
#include <hdb/store/dream_table.hpp>
#include <hdb/store/neuron_table.hpp>
#include <hdb/store/synapse_table.hpp>
#include <hdb/types.hpp>

namespace py = pybind11;

namespace {

struct PyNeuronTable : hdb::NeuronTable {
  using hdb::NeuronTable::NeuronTable;

  std::optional<hdb::Neuron> insert(const hdb::Neuron& n) override {
    PYBIND11_OVERRIDE_PURE(
        std::optional<hdb::Neuron>, hdb::NeuronTable, insert, n);
  }

  std::optional<hdb::Neuron> find_by_id(const hdb::Nid& id) const override {
    PYBIND11_OVERRIDE_PURE(
        std::optional<hdb::Neuron>, hdb::NeuronTable, find_by_id, id);
  }

  std::vector<hdb::Neuron> find_by_range(
      const hdb::Moment since,
      const hdb::Moment until) const override {
    py::gil_scoped_acquire gil;
    py::function override_fn = py::get_override(this, "find_by_range");
    if (!override_fn) {
      pybind11::pybind11_fail(
          "Tried to call pure virtual function NeuronTable::find_by_range");
    }
    const auto dt = py::module_::import("datetime").attr("datetime");
    const py::object py_since =
        (since == hdb::Moment::min()) ? dt.attr("min") : py::cast(since);
    const py::object py_until =
        (until == hdb::Moment::max()) ? dt.attr("max") : py::cast(until);
    return override_fn(py_since, py_until)
        .template cast<std::vector<hdb::Neuron>>();
  }
};

struct PySynapseTable : hdb::SynapseTable {
  using hdb::SynapseTable::SynapseTable;

  std::optional<hdb::Synapse> insert(const hdb::Synapse& s) override {
    PYBIND11_OVERRIDE_PURE(
        std::optional<hdb::Synapse>, hdb::SynapseTable, insert, s);
  }

  std::optional<hdb::Synapse> find_by_id(const hdb::Sid& id) const override {
    PYBIND11_OVERRIDE_PURE(
        std::optional<hdb::Synapse>, hdb::SynapseTable, find_by_id, id);
  }

  std::vector<hdb::Synapse> find_by_range(
      const hdb::Moment since,
      const hdb::Moment until) const override {
    py::gil_scoped_acquire gil;
    py::function override_fn = py::get_override(this, "find_by_range");
    if (!override_fn) {
      pybind11::pybind11_fail(
          "Tried to call pure virtual function SynapseTable::find_by_range");
    }
    const auto dt = py::module_::import("datetime").attr("datetime");
    const py::object py_since =
        (since == hdb::Moment::min()) ? dt.attr("min") : py::cast(since);
    const py::object py_until =
        (until == hdb::Moment::max()) ? dt.attr("max") : py::cast(until);
    return override_fn(py_since, py_until)
        .template cast<std::vector<hdb::Synapse>>();
  }
};

struct PyDreamTable : hdb::DreamTable {
  using hdb::DreamTable::DreamTable;

  std::optional<hdb::Dream> insert(const hdb::Dream& d) override {
    PYBIND11_OVERRIDE_PURE(
        std::optional<hdb::Dream>, hdb::DreamTable, insert, d);
  }

  std::vector<hdb::Resonance> find(
      std::span<const std::byte> payload,
      const hdb::Natural limit) const override {
    py::gil_scoped_acquire gil;
    py::function override_fn = py::get_override(this, "find");
    if (!override_fn) {
      pybind11::pybind11_fail(
          "Tried to call pure virtual function DreamTable::find");
    }
    py::bytes py_payload(
        reinterpret_cast<const char*>(payload.data()),
        static_cast<py::ssize_t>(payload.size()));
    return override_fn(py_payload, limit)
        .template cast<std::vector<hdb::Resonance>>();
  }
};

}  // namespace

PYBIND11_MODULE(_hdb_core, m) {
  m.doc() = "HDB core type bindings";

  py::class_<hdb::Neuron>(m, "Neuron")
      .def(py::init<>())
      .def_readwrite("name", &hdb::Neuron::name)
      .def_readwrite("actor", &hdb::Neuron::actor)
      .def_readwrite("payload", &hdb::Neuron::payload)
      .def_readwrite("moment", &hdb::Neuron::moment)
      .def_readwrite("meta", &hdb::Neuron::meta);

  py::class_<hdb::Synapse>(m, "Synapse")
      .def(py::init<>())
      .def_readwrite("name", &hdb::Synapse::name)
      .def_readwrite("actor", &hdb::Synapse::actor)
      .def_readwrite("source", &hdb::Synapse::source)
      .def_readwrite("target", &hdb::Synapse::target)
      .def_readwrite("moment", &hdb::Synapse::moment)
      .def_readwrite("meta", &hdb::Synapse::meta);

  py::class_<hdb::Dream>(m, "Dream")
      .def(py::init<>())
      .def_readwrite("name", &hdb::Dream::name)
      .def_readwrite("actor", &hdb::Dream::actor)
      .def_readwrite("neuron", &hdb::Dream::neuron)
      .def_readwrite("payload", &hdb::Dream::payload)
      .def_readwrite("moment", &hdb::Dream::moment)
      .def_readwrite("meta", &hdb::Dream::meta);

  py::class_<hdb::Resonance>(m, "Resonance")
      .def(py::init<>())
      .def_readwrite("neuron", &hdb::Resonance::neuron)
      .def_readwrite("fidelity", &hdb::Resonance::fidelity);

  py::class_<hdb::Thought>(m, "Thought")
      .def(py::init<>())
      .def_readwrite("neuron", &hdb::Thought::neuron)
      .def_readwrite("flux", &hdb::Thought::flux);

  py::class_<hdb::Engram>(m, "Engram")
      .def(
          py::init<std::vector<hdb::Neuron>, std::vector<hdb::Synapse>>(),
          py::arg("neurons") = std::vector<hdb::Neuron>{},
          py::arg("synapses") = std::vector<hdb::Synapse>{})
      .def_property_readonly("neurons", &hdb::Engram::Neurons)
      .def_property_readonly("synapses", &hdb::Engram::Synapses);

  py::class_<
      hdb::NeuronTable,
      PyNeuronTable,
      std::shared_ptr<hdb::NeuronTable>>(m, "NeuronTable")
      .def(py::init<>())
      .def("insert", &hdb::NeuronTable::insert, py::arg("neuron"))
      .def("find_by_id", &hdb::NeuronTable::find_by_id, py::arg("id"))
      .def(
          "find_by_range",
          &hdb::NeuronTable::find_by_range,
          py::arg("since"),
          py::arg("until"));

  py::class_<
      hdb::SynapseTable,
      PySynapseTable,
      std::shared_ptr<hdb::SynapseTable>>(m, "SynapseTable")
      .def(py::init<>())
      .def("insert", &hdb::SynapseTable::insert, py::arg("synapse"))
      .def("find_by_id", &hdb::SynapseTable::find_by_id, py::arg("id"))
      .def(
          "find_by_range",
          &hdb::SynapseTable::find_by_range,
          py::arg("since"),
          py::arg("until"));

  py::class_<hdb::DreamTable, PyDreamTable, std::shared_ptr<hdb::DreamTable>>(
      m, "DreamTable")
      .def(py::init<>())
      .def("insert", &hdb::DreamTable::insert, py::arg("dream"))
      .def(
          "find",
          [](const hdb::DreamTable& self,
             const py::bytes& payload,
             const hdb::Natural limit) {
            const char* buf = PyBytes_AS_STRING(payload.ptr());
            const Py_ssize_t len = PyBytes_GET_SIZE(payload.ptr());
            const std::span<const std::byte> sp{
                reinterpret_cast<const std::byte*>(buf),
                static_cast<std::size_t>(len)};
            return self.find(sp, limit);
          },
          py::arg("payload"),
          py::arg("limit") = hdb::Natural{10});
}
