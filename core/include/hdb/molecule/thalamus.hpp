#pragma once

#include <optional>
#include <span>

#include <hdb/atom/dream.hpp>
#include <hdb/store/dream_table.hpp>

namespace hdb {

class Thalamus {
 public:
  explicit Thalamus(DreamTable& table);

  std::optional<Dream> Consolidate(
      std::span<const std::byte> actor,
      const Nid& neuron,
      std::span<const std::byte> payload,
      std::optional<std::span<const std::byte>> meta = std::nullopt);

 private:
  DreamTable& table_;
};

}  // namespace hdb
