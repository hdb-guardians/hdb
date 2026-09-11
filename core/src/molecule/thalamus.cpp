#include <cstddef>
#include <optional>
#include <span>
#include <vector>

#include <hdb/molecule/thalamus.hpp>

namespace hdb {

Thalamus::Thalamus(NeuronTable& neurons, DreamTable& dreams)
    : neurons(neurons), dreams(dreams) {}

std::optional<Dream> Thalamus::Consolidate(
    const Did& name,
    std::span<const std::byte> actor,
    const Nid& neuron,
    std::span<const std::byte> payload,
    std::optional<std::span<const std::byte>> meta) {
  if (!neurons.find_by_id(neuron)) {
    return std::nullopt;
  }

  Dream dream{
      .name = name,
      .actor = {actor.begin(), actor.end()},
      .neuron = neuron,
      .payload = {payload.begin(), payload.end()},
      .moment = std::chrono::time_point_cast<Moment::duration>(Clock::now()),
      .meta = std::nullopt,
  };

  if (meta) {
    dream.meta = std::vector<std::byte>{meta->begin(), meta->end()};
  }

  return dreams.insert(dream);
}

}  // namespace hdb
