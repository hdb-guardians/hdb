#include <hdb/molecule/hippocampus.hpp>

namespace hdb {

Hippocampus::Hippocampus(
    NeuronTable& neurons,
    SynapseTable& synapses,
    DreamTable& abstracts)
    : neurons(neurons), synapses(synapses), dreams(abstracts) {}

std::vector<Resonance> Hippocampus::Resonate(
    std::span<const std::byte> stimulus,
    const Natural limit) {
  return dreams.find(stimulus, limit);
}

std::optional<Engram> Hippocampus::Reminisce(
    const Moment since,
    const Moment until) {
  auto _neurons = neurons.find(since, until);
  auto _synapses = synapses.find(since, until);

  if (_neurons.empty() && _synapses.empty()) {
    return std::nullopt;
  }

  return Engram{std::move(_neurons), std::move(_synapses)};
}

}
