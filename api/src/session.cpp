#include <hdb/api/session.hpp>

namespace hdb::api {

Session::Session(
    std::shared_ptr<NeuronTable> neurons,
    std::shared_ptr<SynapseTable> synapses,
    std::shared_ptr<DreamTable> dreams)
    : neurons_(std::move(neurons)),
      synapses_(std::move(synapses)),
      dreams_(std::move(dreams)),
      prefrontal_(*neurons_, *synapses_),
      thalamus_(*dreams_),
      hippocampus_(*neurons_, *synapses_, *dreams_) {}

std::optional<Neuron> Session::Sprout(
    const Nid& name,
    std::span<const std::byte> actor,
    std::span<const std::byte> payload,
    std::optional<std::span<const std::byte>> meta) {
  return prefrontal_.Sprout(name, actor, payload, meta);
}

std::optional<Neuron> Session::Awaken(const Nid& name) {
  return prefrontal_.Awaken(name);
}

std::optional<Synapse> Session::Fire(
    const Sid& name,
    std::span<const std::byte> actor,
    const Nid& source,
    const Nid& target,
    std::optional<std::span<const std::byte>> meta) {
  return prefrontal_.Fire(name, actor, source, target, meta);
}

std::optional<Dream> Session::Consolidate(
    const Did& name,
    std::span<const std::byte> actor,
    const Nid& neuron,
    std::span<const std::byte> payload,
    std::optional<std::span<const std::byte>> meta) {
  return thalamus_.Consolidate(name, actor, neuron, payload, meta);
}

std::vector<Resonance> Session::Resonate(
    std::span<const std::byte> stimulus,
    const std::size_t limit) {
  return hippocampus_.Resonate(stimulus, limit);
}

std::optional<Engram> Session::Reminisce(
    const Moment since,
    const Moment until) {
  return hippocampus_.Reminisce(since, until);
}

Imagination Session::Imagine(
    const Engram& engram,
    const Nid& start,
    const std::size_t epochs,
    const Real creativity,
    const Impulse& impulse) {
  return cortex_.Imagine(engram, start, epochs, creativity, impulse);
}

}  // namespace hdb::api
