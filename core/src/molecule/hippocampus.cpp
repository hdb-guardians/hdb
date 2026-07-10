#include <cstddef>
#include <optional>
#include <span>
#include <utility>
#include <vector>

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
  auto neurons_ = neurons.find(since, until);
  auto synapses_ = synapses.find(since, until);

  if (neurons_.empty() && synapses_.empty()) {
    return std::nullopt;
  }

  return Engram{std::move(neurons_), std::move(synapses_)};
}

}  // namespace hdb
