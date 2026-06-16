#include <hdb/molecule/thalamus.hpp>

#include <hdb/types.hpp>

namespace hdb {

Thalamus::Thalamus(DreamTable& table) : dreams(table) {}

std::optional<Dream> Thalamus::Consolidate(
    const Did& name,
    std::span<const std::byte> actor,
    const Nid& neuron,
    std::span<const std::byte> payload,
    std::optional<std::span<const std::byte>> meta) {
  Dream d;
  d.name = name;
  d.actor = {actor.begin(), actor.end()};
  d.neuron = neuron;
  d.payload = {payload.begin(), payload.end()};
  d.moment = Clock::now();

  if (meta) {
    d.meta = std::vector<std::byte>{meta->begin(), meta->end()};
  }

  return dreams.insert(d);
}

}  // namespace hdb
