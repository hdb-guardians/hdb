#include <cstddef>
#include <optional>
#include <span>
#include <vector>

#include <hdb/molecule/prefrontal.hpp>

namespace hdb {

Prefrontal::Prefrontal(NeuronTable& neurons, SynapseTable& synapses)
    : neurons(neurons), synapses(synapses) {}

std::optional<Neuron> Prefrontal::Sprout(
    const Nid& name,
    std::span<const std::byte> actor,
    std::span<const std::byte> payload,
    std::optional<std::span<const std::byte>> meta) {
  Neuron neuron{
      .name = name,
      .actor = {actor.begin(), actor.end()},
      .payload = {payload.begin(), payload.end()},
      .moment = Clock::now(),
      .meta = std::nullopt,
  };

  if (meta) {
    neuron.meta = std::vector<std::byte>{meta->begin(), meta->end()};
  }

  return neurons.insert(neuron);
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
  Synapse synapse{
      .name = name,
      .actor = {actor.begin(), actor.end()},
      .from = from,
      .to = to,
      .moment = Clock::now(),
      .meta = std::nullopt,
  };

  if (meta) {
    synapse.meta = std::vector<std::byte>{meta->begin(), meta->end()};
  }

  return synapses.insert(synapse);
}

}  // namespace hdb
