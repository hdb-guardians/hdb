#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <hdb/standard/session.hpp>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <utility>
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

std::optional<std::vector<std::byte>> ToOptionalBytes(const py::object& obj) {
  if (obj.is_none()) {
    return std::nullopt;
  }
  return ToBytes(obj.cast<py::bytes>());
}

py::object FromOptionalBytes(const std::optional<std::vector<std::byte>>& obj) {
  if (!obj.has_value()) {
    return py::none();
  }
  return FromBytes(*obj);
}

std::optional<std::span<const std::byte>> OptionalSpanFromOwned(
    const std::optional<std::vector<std::byte>>& owned) {
  if (!owned.has_value()) {
    return std::nullopt;
  }
  return std::span<const std::byte>(*owned);
}

std::int64_t ToMomentTicks(const hdb::Moment moment) {
  return static_cast<std::int64_t>(moment.time_since_epoch().count());
}

hdb::Moment FromMomentTicks(const std::int64_t ticks) {
  return hdb::Moment{hdb::Clock::duration{ticks}};
}

hdb::Moment ReadMomentFromDict(const py::dict& obj) {
  if (obj.contains("moment_ticks")) {
    return FromMomentTicks(obj["moment_ticks"].cast<std::int64_t>());
  }

  if (obj.contains("moment_ms")) {
    using namespace std::chrono;
    return hdb::Moment{milliseconds{obj["moment_ms"].cast<std::int64_t>()}};
  }

  throw std::runtime_error(
      "hdb::python::read_moment: invalid input: moment_ticks is required");
}

py::dict NeuronToDict(const hdb::Neuron& n) {
  py::dict out;
  out["name"] = n.name;
  out["actor"] = FromBytes(n.actor);
  out["payload"] = FromBytes(n.payload);
  out["moment_ticks"] = ToMomentTicks(n.moment);
  out["meta"] = FromOptionalBytes(n.meta);
  return out;
}

py::dict SynapseToDict(const hdb::Synapse& s) {
  py::dict out;
  out["name"] = s.name;
  out["actor"] = FromBytes(s.actor);
  out["from"] = s.from;
  out["to"] = s.to;
  out["moment_ticks"] = ToMomentTicks(s.moment);
  out["meta"] = FromOptionalBytes(s.meta);
  return out;
}

py::dict DreamToDict(const hdb::Dream& d) {
  py::dict out;
  out["name"] = d.name;
  out["actor"] = FromBytes(d.actor);
  out["neuron"] = d.neuron;
  out["payload"] = FromBytes(d.payload);
  out["moment_ticks"] = ToMomentTicks(d.moment);
  out["meta"] = FromOptionalBytes(d.meta);
  return out;
}

hdb::Neuron DictToNeuron(const py::dict& obj) {
  hdb::Neuron out;
  out.name = obj["name"].cast<std::string>();
  out.actor = ToBytes(obj["actor"].cast<py::bytes>());
  out.payload = ToBytes(obj["payload"].cast<py::bytes>());
  out.moment = ReadMomentFromDict(obj);
  if (obj.contains("meta") && !obj["meta"].is_none()) {
    out.meta = ToBytes(obj["meta"].cast<py::bytes>());
  }
  return out;
}

hdb::Synapse DictToSynapse(const py::dict& obj) {
  hdb::Synapse out;
  out.name = obj["name"].cast<std::string>();
  out.actor = ToBytes(obj["actor"].cast<py::bytes>());
  out.from = obj["from"].cast<std::string>();
  out.to = obj["to"].cast<std::string>();
  out.moment = ReadMomentFromDict(obj);
  if (obj.contains("meta") && !obj["meta"].is_none()) {
    out.meta = ToBytes(obj["meta"].cast<py::bytes>());
  }
  return out;
}

hdb::Engram DictToEngram(const py::dict& obj) {
  std::vector<hdb::Neuron> neurons;
  std::vector<hdb::Synapse> synapses;

  if (obj.contains("neurons")) {
    for (const auto& item : obj["neurons"].cast<py::list>()) {
      neurons.push_back(DictToNeuron(py::reinterpret_borrow<py::dict>(item)));
    }
  }

  if (obj.contains("synapses")) {
    for (const auto& item : obj["synapses"].cast<py::list>()) {
      synapses.push_back(DictToSynapse(py::reinterpret_borrow<py::dict>(item)));
    }
  }

  return hdb::Engram{std::move(neurons), std::move(synapses)};
}

py::dict ThoughtToDict(const hdb::Thought& thought) {
  py::dict out;
  out["neuron"] = NeuronToDict(thought.neuron);
  out["flux"] = thought.flux;
  return out;
}

hdb::Impulse BuildImpulse(const py::object& impulse_obj) {
  if (impulse_obj.is_none()) {
    return [](const hdb::Synapse&) { return hdb::Real{1}; };
  }

  const py::function impulse_fn = impulse_obj.cast<py::function>();
  return [impulse_fn](const hdb::Synapse& synapse) -> hdb::Real {
    py::gil_scoped_acquire gil;
    const py::object value = impulse_fn(SynapseToDict(synapse));
    return value.cast<hdb::Real>();
  };
}

}

PYBIND11_MODULE(_hdb, m) {
  m.doc() = "HDB standard runtime bindings";

  py::class_<hdb::standard::Session>(m, "Session")
      .def(
          py::init<const std::string&, const std::string&>(),
          py::arg("db_path"),
          py::arg("sqlite_vec_extension_path") = "")
      .def(
          "sprout",
          [](hdb::standard::Session& self,
             const std::string& name,
             const py::bytes actor,
             const py::bytes payload,
             const py::object meta_obj) {
            const auto actor_bytes = ToBytes(actor);
            const auto payload_bytes = ToBytes(payload);
            const auto meta_bytes = ToOptionalBytes(meta_obj);
            const auto meta_span = OptionalSpanFromOwned(meta_bytes);

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
            const auto meta_bytes = ToOptionalBytes(meta_obj);
            const auto meta_span = OptionalSpanFromOwned(meta_bytes);

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
            const auto meta_bytes = ToOptionalBytes(meta_obj);
            const auto meta_span = OptionalSpanFromOwned(meta_bytes);

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
             const std::int64_t since_ticks,
             const std::int64_t until_ticks) {
            auto out = self.Reminisce(
                FromMomentTicks(since_ticks), FromMomentTicks(until_ticks));
            if (!out.has_value()) {
              return py::object(py::none());
            }

            py::dict result;
            py::list neurons;
            for (const auto& n : out->Neurons()) {
              neurons.append(NeuronToDict(n));
            }

            py::list synapses;
            for (const auto& s : out->Synapses()) {
              synapses.append(SynapseToDict(s));
            }

            result["neurons"] = neurons;
            result["synapses"] = synapses;
            return py::object(result);
          },
          py::arg("since_ticks") = ToMomentTicks(hdb::Moment::min()),
          py::arg("until_ticks") = ToMomentTicks(hdb::Moment::max()))
      .def(
          "imagine",
          [](hdb::standard::Session& self,
             const py::dict engram_obj,
             const std::string& start,
             const std::size_t epochs,
             const hdb::Real creativity,
             const py::object impulse_obj) {
            const hdb::Engram engram = DictToEngram(engram_obj);
            const hdb::Impulse impulse = BuildImpulse(impulse_obj);
            const hdb::Imagination out =
                self.Imagine(engram, start, epochs, creativity, impulse);

            py::list result;
            for (const auto& thought : out) {
              result.append(ThoughtToDict(thought));
            }
            return result;
          },
          py::arg("engram"),
          py::arg("start"),
          py::arg("epochs"),
          py::arg("creativity"),
          py::arg("impulse") = py::none());
}
