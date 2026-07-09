#include <cstddef>
#include <optional>
#include <span>
#include <vector>

#include <hdb/molecule/thalamus.hpp>

namespace hdb {

Thalamus::Thalamus(DreamTable& table) : dreams(table) {}

std::optional<Dream> Thalamus::Consolidate(
    const Did& name,
    std::span<const std::byte> actor,
    const Nid& neuron,
    std::span<const std::byte> payload,
    std::optional<std::span<const std::byte>> meta) {
  Dream dream{
      .name = name,
      .actor = {actor.begin(), actor.end()},
      .neuron = neuron,
      .payload = {payload.begin(), payload.end()},
      .moment = Clock::now(),
  };

  if (meta) {
    dream.meta = std::vector<std::byte>{meta->begin(), meta->end()};
  }

  return dreams.insert(dream);
}

}
