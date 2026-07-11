#include <pybind11/chrono.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cstddef>
#include <memory>
#include <optional>
#include <span>
#include <vector>

#include <hdb/api/session.hpp>
#include <hdb/atom/engram.hpp>
#include <hdb/atom/imagination.hpp>
#include <hdb/atom/impulse.hpp>
#include <hdb/python/byte_caster.hpp>
#include <hdb/quark/neuron.hpp>
#include <hdb/quark/resonance.hpp>
#include <hdb/quark/synapse.hpp>
#include <hdb/store/dream_table.hpp>
#include <hdb/store/neuron_table.hpp>
#include <hdb/store/synapse_table.hpp>
#include <hdb/types.hpp>

namespace py = pybind11;

namespace {

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

PYBIND11_MODULE(_hdb_api, m) {
  m.doc() = "HDB API bindings";

  py::class_<hdb::api::Session>(m, "Session")
      .def(
          py::init([](std::shared_ptr<hdb::NeuronTable> neurons,
                      std::shared_ptr<hdb::SynapseTable> synapses,
                      std::shared_ptr<hdb::DreamTable> dreams) {
            return std::make_unique<hdb::api::Session>(
                std::move(neurons), std::move(synapses), std::move(dreams));
          }),
          py::arg("neurons"),
          py::arg("synapses"),
          py::arg("dreams"),
          py::keep_alive<1, 2>(),
          py::keep_alive<1, 3>(),
          py::keep_alive<1, 4>())
      .def(
          "sprout",
          [](hdb::api::Session& self,
             const hdb::Nid& name,
             std::vector<std::byte> actor,
             std::vector<std::byte> payload,
             std::optional<std::vector<std::byte>> meta)
              -> std::optional<hdb::Neuron> {
            std::optional<std::span<std::byte>> meta_span;
            if (meta.has_value()) {
              meta_span = std::span<std::byte>(*meta);
            }
            return self.Sprout(name, actor, payload, meta_span);
          },
          py::arg("name"),
          py::arg("actor"),
          py::arg("payload"),
          py::arg("meta") = py::none())
      .def(
          "awaken",
          [](hdb::api::Session& self, const hdb::Nid& name)
              -> std::optional<hdb::Neuron> { return self.Awaken(name); },
          py::arg("name"))
      .def(
          "fire",
          [](hdb::api::Session& self,
             const hdb::Sid& name,
             std::vector<std::byte> actor,
             const hdb::Nid& source,
             const hdb::Nid& target,
             std::optional<std::vector<std::byte>> meta)
              -> std::optional<hdb::Synapse> {
            std::optional<std::span<std::byte>> meta_span;
            if (meta.has_value()) {
              meta_span = std::span<std::byte>(*meta);
            }
            return self.Fire(name, actor, source, target, meta_span);
          },
          py::arg("name"),
          py::arg("actor"),
          py::arg("source"),
          py::arg("target"),
          py::arg("meta") = py::none())
      .def(
          "consolidate",
          [](hdb::api::Session& self,
             const hdb::Did& name,
             std::vector<std::byte> actor,
             const hdb::Nid& neuron,
             std::vector<std::byte> payload,
             std::optional<std::vector<std::byte>> meta)
              -> std::optional<hdb::Dream> {
            std::optional<std::span<std::byte>> meta_span;
            if (meta.has_value()) {
              meta_span = std::span<std::byte>(*meta);
            }
            return self.Consolidate(name, actor, neuron, payload, meta_span);
          },
          py::arg("name"),
          py::arg("actor"),
          py::arg("neuron"),
          py::arg("payload"),
          py::arg("meta") = py::none())
      .def(
          "resonate",
          [](hdb::api::Session& self,
             std::vector<std::byte> stimulus,
             const std::size_t limit) -> std::vector<hdb::Resonance> {
            return self.Resonate(stimulus, limit);
          },
          py::arg("stimulus"),
          py::arg("limit") = std::size_t{10})
      .def(
          "reminisce",
          [](hdb::api::Session& self,
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
          py::arg("until") = py::none())
      .def(
          "imagine",
          [](hdb::api::Session& self,
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