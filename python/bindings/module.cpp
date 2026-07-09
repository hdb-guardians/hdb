#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <chrono>
#include <cstring>
#include <hdb/standard/session.hpp>
#include <optional>
#include <string>
#include <vector>

namespace py = pybind11;

namespace {

std::vector<std::byte> ToBytes(const py::bytes& raw) {
  const std::string s = raw;
  std::vector<std::byte> out(s.size());
  if (!s.empty()) {
    std::memcpy(out.data(), s.data(), s.size());
  }
  return out;
}

py::bytes FromBytes(const std::vector<std::byte>& raw) {
  if (raw.empty()) {
    return py::bytes();
  }

  return py::bytes(
      reinterpret_cast<const char*>(raw.data()),
      static_cast<py::ssize_t>(raw.size()));
}

std::int64_t ToEpochMs(const hdb::Moment moment) {
  using namespace std::chrono;
  return duration_cast<milliseconds>(moment.time_since_epoch()).count();
}

hdb::Moment FromEpochMs(const std::int64_t ms) {
  using namespace std::chrono;
  return hdb::Moment{milliseconds{ms}};
}

py::dict NeuronToDict(const hdb::Neuron& n) {
  py::dict out;
  out["name"] = n.name;
  out["actor"] = FromBytes(n.actor);
  out["payload"] = FromBytes(n.payload);
  out["moment_ms"] = ToEpochMs(n.moment);
  if (n.meta.has_value()) {
    out["meta"] = FromBytes(*n.meta);
  } else {
    out["meta"] = py::none();
  }
  return out;
}

py::dict SynapseToDict(const hdb::Synapse& s) {
  py::dict out;
  out["name"] = s.name;
  out["actor"] = FromBytes(s.actor);
  out["from"] = s.from;
  out["to"] = s.to;
  out["moment_ms"] = ToEpochMs(s.moment);
  if (s.meta.has_value()) {
    out["meta"] = FromBytes(*s.meta);
  } else {
    out["meta"] = py::none();
  }
  return out;
}

py::dict DreamToDict(const hdb::Dream& d) {
  py::dict out;
  out["name"] = d.name;
  out["actor"] = FromBytes(d.actor);
  out["neuron"] = d.neuron;
  out["payload"] = FromBytes(d.payload);
  out["moment_ms"] = ToEpochMs(d.moment);
  if (d.meta.has_value()) {
    out["meta"] = FromBytes(*d.meta);
  } else {
    out["meta"] = py::none();
  }
  return out;
}

}  // namespace

PYBIND11_MODULE(_hdb, m) {
  m.doc() = "HDB standard runtime bindings";

  py::class_<hdb::standard::Session>(m, "Session")
      .def(
          py::init<const std::string&, std::optional<std::string>>(),
          py::arg("db_path"),
          py::arg("sqlite_vec_extension_path") = std::nullopt)
      .def(
          "sprout",
          [](hdb::standard::Session& self,
             const std::string& name,
             const py::bytes actor,
             const py::bytes payload,
             const py::object meta_obj) {
            const auto actor_bytes = ToBytes(actor);
            const auto payload_bytes = ToBytes(payload);

            std::optional<std::vector<std::byte>> meta_bytes;
            if (!meta_obj.is_none()) {
              meta_bytes = ToBytes(meta_obj.cast<py::bytes>());
            }

            std::optional<std::span<const std::byte>> meta_span = std::nullopt;
            if (meta_bytes.has_value()) {
              meta_span = std::span<const std::byte>(*meta_bytes);
            }

            auto out = self.Sprout(name, actor_bytes, payload_bytes, meta_span);
            if (!out.has_value()) {
              return py::object(py::none());
            }
            return py::object(NeuronToDict(*out));
          },
          py::arg("name"),
          py::arg("actor"),
          py::arg("payload"),
          py::arg("meta") = py::none())
      .def(
          "awaken",
          [](hdb::standard::Session& self, const std::string& name) {
            auto out = self.Awaken(name);
            if (!out.has_value()) {
              return py::object(py::none());
            }
            return py::object(NeuronToDict(*out));
          },
          py::arg("name"))
      .def(
          "fire",
          [](hdb::standard::Session& self,
             const std::string& name,
             const py::bytes actor,
             const std::string& from,
             const std::string& to,
             const py::object meta_obj) {
            const auto actor_bytes = ToBytes(actor);

            std::optional<std::vector<std::byte>> meta_bytes;
            if (!meta_obj.is_none()) {
              meta_bytes = ToBytes(meta_obj.cast<py::bytes>());
            }

            std::optional<std::span<const std::byte>> meta_span = std::nullopt;
            if (meta_bytes.has_value()) {
              meta_span = std::span<const std::byte>(*meta_bytes);
            }

            auto out = self.Fire(name, actor_bytes, from, to, meta_span);
            if (!out.has_value()) {
              return py::object(py::none());
            }
            return py::object(SynapseToDict(*out));
          },
          py::arg("name"),
          py::arg("actor"),
          py::arg("from"),
          py::arg("to"),
          py::arg("meta") = py::none())
      .def(
          "consolidate",
          [](hdb::standard::Session& self,
             const std::string& name,
             const py::bytes actor,
             const std::string& neuron,
             const py::bytes payload,
             const py::object meta_obj) {
            const auto actor_bytes = ToBytes(actor);
            const auto payload_bytes = ToBytes(payload);

            std::optional<std::vector<std::byte>> meta_bytes;
            if (!meta_obj.is_none()) {
              meta_bytes = ToBytes(meta_obj.cast<py::bytes>());
            }

            std::optional<std::span<const std::byte>> meta_span = std::nullopt;
            if (meta_bytes.has_value()) {
              meta_span = std::span<const std::byte>(*meta_bytes);
            }

            auto out = self.Consolidate(
                name, actor_bytes, neuron, payload_bytes, meta_span);
            if (!out.has_value()) {
              return py::object(py::none());
            }
            return py::object(DreamToDict(*out));
          },
          py::arg("name"),
          py::arg("actor"),
          py::arg("neuron"),
          py::arg("payload"),
          py::arg("meta") = py::none())
      .def(
          "resonate",
          [](hdb::standard::Session& self,
             const py::bytes stimulus,
             const std::size_t limit) {
            const auto stimulus_bytes = ToBytes(stimulus);
            const auto out = self.Resonate(stimulus_bytes, limit);

            py::list rows;
            for (const auto& r : out) {
              rows.append(py::make_tuple(r.neuron, r.fidelity));
            }
            return rows;
          },
          py::arg("stimulus"),
          py::arg("limit") = 10)
      .def(
          "reminisce",
          [](hdb::standard::Session& self,
             const std::int64_t since_ms,
             const std::int64_t until_ms) {
            auto out =
                self.Reminisce(FromEpochMs(since_ms), FromEpochMs(until_ms));
            if (!out.has_value()) {
              return py::object(py::none());
            }

            py::dict result;
            py::list neurons;
            for (const auto& n : out->neurons) {
              neurons.append(NeuronToDict(n));
            }

            py::list synapses;
            for (const auto& s : out->synapses) {
              synapses.append(SynapseToDict(s));
            }

            result["neurons"] = neurons;
            result["synapses"] = synapses;
            return py::object(result);
          },
          py::arg("since_ms"),
          py::arg("until_ms"));
}
