#pragma once

#include <optional>
#include <span>

#include <hdb/atom/abstract.hpp>
#include <hdb/store/abstract_table.hpp>

namespace hdb {

class Thalamus {
 public:
  explicit Thalamus(AbstractTable& table);

  std::optional<Abstract> Consolidate(
      std::span<const std::byte> actor,
      const Nid& neuron,
      std::span<const std::byte> payload,
      std::optional<std::span<const std::byte>> meta = std::nullopt);

 private:
  AbstractTable& table_;
};

}  // namespace hdb
