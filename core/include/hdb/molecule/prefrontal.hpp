#pragma once

#include <cstddef>
#include <optional>
#include <span>

#include <hdb/atom/neuron.hpp>
#include <hdb/atom/synapse.hpp>
#include <hdb/store/ineuron_table.hpp>
#include <hdb/store/isynapse_table.hpp>

namespace hdb {

class Prefrontal {
 public:
  Prefrontal(INeuronTable& neurons, ISynapseTable& synapses);

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
  INeuronTable& neurons_;
  ISynapseTable& synapses_;
};

}  // namespace hdb
