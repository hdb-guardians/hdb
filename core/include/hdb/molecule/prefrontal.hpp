#pragma once

#include <optional>
#include <span>

#include <hdb/atom/neuron.hpp>
#include <hdb/atom/synapse.hpp>
#include <hdb/store/neuron_table.hpp>
#include <hdb/store/synapse_table.hpp>

namespace hdb {

class Prefrontal {
 public:
  explicit Prefrontal(NeuronTable& neurons, SynapseTable& synapses);

  std::optional<Neuron> Sprout(
      std::span<const std::byte> actor,
      std::span<const std::byte> payload,
      std::optional<std::span<const std::byte>> meta = std::nullopt);

  std::optional<Neuron> Awaken(const Nid& name);

  std::optional<Synapse> Fire(
      std::span<const std::byte> actor,
      const Nid& from,
      const Nid& to,
      std::optional<std::span<const std::byte>> meta = std::nullopt);

 private:
  NeuronTable& neurons_;
  SynapseTable& synapses_;
};

}  // namespace hdb
