#include <hdb/molecule/prefrontal.hpp>

#include <hdb/types.hpp>

namespace hdb {

Prefrontal::Prefrontal(NeuronTable& neurons, SynapseTable& synapses)
    : neurons(neurons), synapses(synapses) {}

std::optional<Neuron> Prefrontal::Sprout(
    const Nid& name,
    std::span<const std::byte> actor,
    std::span<const std::byte> payload,
    std::optional<std::span<const std::byte>> meta) {
  Neuron n;
  n.name = name;
  n.actor = {actor.begin(), actor.end()};
  n.payload = {payload.begin(), payload.end()};
  n.moment = Clock::now();

  if (meta) {
    n.meta = std::vector<std::byte>{meta->begin(), meta->end()};
  }

  return neurons.insert(n);
}

std::optional<Neuron> Prefrontal::Awaken(const Nid& name) {
  return neurons.find(name);
}

std::optional<Synapse> Prefrontal::Fire(
    const Sid& name,
    std::span<const std::byte> actor,
    const Nid& from,
    const Nid& to,
    std::optional<std::span<const std::byte>> meta) {
  Synapse s;
  s.name = name;
  s.actor = {actor.begin(), actor.end()};
  s.from = from;
  s.to = to;
  s.moment = Clock::now();

  if (meta) {
    s.meta = std::vector<std::byte>{meta->begin(), meta->end()};
  }

  return synapses.insert(s);
}

}  // namespace hdb
