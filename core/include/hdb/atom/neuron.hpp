#pragma once

#include <cstddef>
#include <optional>
#include <span>
#include <vector>

#include <hdb/types.hpp>
#include <hdb/store/ineuron_table.hpp>

namespace hdb {

class Neuron {
 public:
  explicit Neuron(INeuronTable& table);

  std::optional<Nid> Sprout(
      std::span<const std::byte> actor,
      std::span<const std::byte> payload,
      std::optional<std::span<const std::byte>> meta = std::nullopt);

  std::optional<NeuronRecord> Awaken(const Nid& nid);

 private:
  INeuronTable& table_;
};

}  // namespace hdb
