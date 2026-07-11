#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cstddef>
#include <memory>
#include <optional>
#include <span>
#include <vector>

#include <hdb/atom/engram.hpp>
#include <hdb/atom/impulse.hpp>
#include <hdb/atom/thought.hpp>
#include <hdb/molecule/cortex.hpp>
#include <hdb/molecule/hippocampus.hpp>
#include <hdb/molecule/prefrontal.hpp>
#include <hdb/molecule/thalamus.hpp>
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
      const std::size_t limit) const override {
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

hdb::Impulse BuildImpulse(const py::object& impulse_obj) {
  if (impulse_obj.is_none()) {
    return [](const hdb::Synapse&) { return hdb::Real{1}; };
  }
  const py::function impulse_fn = impulse_obj.cast<py::function>();
  return [impulse_fn](const hdb::Synapse& synapse) -> hdb::Real {
    py::gil_scoped_acquire gil;
    return impulse_fn(py::cast(synapse)).cast<hdb::Real>();
  };
}

}  // namespace

PYBIND11_MODULE(_hdb_core, m) {
  m.doc() = "HDB core bindings";

  // ── quark ────────────────────────────────────────────────────────────────

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
             const std::size_t limit) {
            const char* buf = PyBytes_AS_STRING(payload.ptr());
            const Py_ssize_t len = PyBytes_GET_SIZE(payload.ptr());
            const std::span<const std::byte> sp{
                reinterpret_cast<const std::byte*>(buf),
                static_cast<std::size_t>(len)};
            return self.find(sp, limit);
          },
          py::arg("payload"),
          py::arg("limit") = std::size_t{10});

  // ── molecule ──────────────────────────────────────────────────────────────
  py::class_<hdb::Prefrontal>(m, "Prefrontal")
      .def(
          py::init([](std::shared_ptr<hdb::NeuronTable> neurons,
                      std::shared_ptr<hdb::SynapseTable> synapses) {
            return std::make_unique<hdb::Prefrontal>(*neurons, *synapses);
          }),
          py::arg("neurons"),
          py::arg("synapses"),
          py::keep_alive<1, 2>(),
          py::keep_alive<1, 3>())
      .def(
          "sprout",
          [](hdb::Prefrontal& self,
             const hdb::Nid& name,
             std::vector<std::byte> actor,
             std::vector<std::byte> payload,
             std::optional<std::vector<std::byte>> meta)
              -> std::optional<hdb::Neuron> {
            std::optional<std::span<const std::byte>> meta_span;
            if (meta.has_value()) {
              meta_span = std::span<const std::byte>(*meta);
            }
            return self.Sprout(name, actor, payload, meta_span);
          },
          py::arg("name"),
          py::arg("actor"),
          py::arg("payload"),
          py::arg("meta") = py::none())
      .def(
          "awaken",
          [](hdb::Prefrontal& self, const hdb::Nid& name) {
            return self.Awaken(name);
          },
          py::arg("name"))
      .def(
          "fire",
          [](hdb::Prefrontal& self,
             const hdb::Sid& name,
             std::vector<std::byte> actor,
             const hdb::Nid& source,
             const hdb::Nid& target,
             std::optional<std::vector<std::byte>> meta)
              -> std::optional<hdb::Synapse> {
            std::optional<std::span<const std::byte>> meta_span;
            if (meta.has_value()) {
              meta_span = std::span<const std::byte>(*meta);
            }
            return self.Fire(name, actor, source, target, meta_span);
          },
          py::arg("name"),
          py::arg("actor"),
          py::arg("source"),
          py::arg("target"),
          py::arg("meta") = py::none());

  py::class_<hdb::Thalamus>(m, "Thalamus")
      .def(
          py::init([](std::shared_ptr<hdb::DreamTable> dreams) {
            return std::make_unique<hdb::Thalamus>(*dreams);
          }),
          py::arg("dreams"),
          py::keep_alive<1, 2>())
      .def(
          "consolidate",
          [](hdb::Thalamus& self,
             const hdb::Did& name,
             std::vector<std::byte> actor,
             const hdb::Nid& neuron,
             std::vector<std::byte> payload,
             std::optional<std::vector<std::byte>> meta)
              -> std::optional<hdb::Dream> {
            std::optional<std::span<const std::byte>> meta_span;
            if (meta.has_value()) {
              meta_span = std::span<const std::byte>(*meta);
            }
            return self.Consolidate(name, actor, neuron, payload, meta_span);
          },
          py::arg("name"),
          py::arg("actor"),
          py::arg("neuron"),
          py::arg("payload"),
          py::arg("meta") = py::none());

  py::class_<hdb::Hippocampus>(m, "Hippocampus")
      .def(
          py::init([](std::shared_ptr<hdb::NeuronTable> neurons,
                      std::shared_ptr<hdb::SynapseTable> synapses,
                      std::shared_ptr<hdb::DreamTable> dreams) {
            return std::make_unique<hdb::Hippocampus>(
                *neurons, *synapses, *dreams);
          }),
          py::arg("neurons"),
          py::arg("synapses"),
          py::arg("dreams"),
          py::keep_alive<1, 2>(),
          py::keep_alive<1, 3>(),
          py::keep_alive<1, 4>())
      .def(
          "resonate",
          [](hdb::Hippocampus& self,
             std::vector<std::byte> stimulus,
             const std::size_t limit) -> std::vector<hdb::Resonance> {
            return self.Resonate(stimulus, limit);
          },
          py::arg("stimulus"),
          py::arg("limit") = std::size_t{10})
      .def(
          "reminisce",
          [](hdb::Hippocampus& self,
             const py::object& since_obj,
             const py::object& until_obj) -> std::optional<hdb::Engram> {
            const hdb::Moment since = since_obj.is_none()
                                          ? hdb::Moment::min()
                                          : since_obj.cast<hdb::Moment>();
            const hdb::Moment until = until_obj.is_none()
                                          ? hdb::Moment::max()
                                          : until_obj.cast<hdb::Moment>();
            return self.Reminisce(since, until);
          },
          py::arg("since") = py::none(),
          py::arg("until") = py::none());

  py::class_<hdb::Cortex>(m, "Cortex")
      .def(py::init<>())
      .def(
          "imagine",
          [](hdb::Cortex& self,
             const hdb::Engram& engram,
             const hdb::Nid& start,
             const std::size_t epochs,
             const hdb::Real creativity,
             const py::object& impulse_obj) -> hdb::Imagination {
            return self.Imagine(
                engram, start, epochs, creativity, BuildImpulse(impulse_obj));
          },
          py::arg("engram"),
          py::arg("start"),
          py::arg("epochs"),
          py::arg("creativity"),
          py::arg("impulse") = py::none());
}
