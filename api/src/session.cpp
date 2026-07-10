#include <utility>

#include <hdb/api/session.hpp>

namespace hdb::api {

Session::Session(Context ctx)
    : ctx_(std::move(ctx)),
      neurons_(ctx_.get<NeuronTable>()),
      synapses_(ctx_.get<SynapseTable>()),
      dreams_(ctx_.get<DreamTable>()),
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
    const Nid& from,
    const Nid& to,
    std::optional<std::span<const std::byte>> meta) {
  return prefrontal_.Fire(name, actor, from, to, meta);
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
    const Natural limit) {
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
    const Natural epochs,
    const Real creativity,
    const Impulse& impulse) {
  return cortex_.Imagine(engram, start, epochs, creativity, impulse);
}

}
